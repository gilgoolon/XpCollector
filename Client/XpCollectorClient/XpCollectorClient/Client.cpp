#include <stdexcept>
#include <chrono>
#include <thread>

#include <httplib.h>

#include "Client.h"
#include "Protocol/BasicRequest.h"
#include "Protocol/InstallClientResponse.h"
#include "Protocol/GetCommandResponse.h"
#include "CommandHandlers/CommandHandlerFactory.h"
#include "WinUtils.h"
using namespace xp_collector;

Client::Client(
	std::unique_ptr<ICommunicator> communicator,
	std::unique_ptr<IClientStorage> storage,
	std::unordered_map<std::unique_ptr<IEvent>, std::vector<std::unique_ptr<IEventHandler>>>&& events,
	std::unique_ptr<ILogger> logger)
	: m_communicator(std::move(communicator))
	  , m_storage(std::move(storage))
	  , m_logger(std::move(logger))
	  , m_events(std::move(events))
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

	for (const auto& item : m_events) {
		std::thread event_detection_thread(&Client::event_detection_loop, this, std::ref(item.first),
		                                   std::ref(item.second));
		event_detection_thread.detach();
	}

	execute_commands_loop();
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
}

bool Client::is_installed() const
{
	return m_storage->has_field(CLIENT_ID_STORAGE_NAME);
}

void Client::event_detection_loop(const std::unique_ptr<IEvent>& event_to_detect,
                                  const std::vector<std::unique_ptr<IEventHandler>>& handlers) const
{
	while (true) {
		if (const auto event_info = event_to_detect->is_detected(); EventType::NotDetected != event_info->get_type()) {
			for (const auto& handler : handlers) {
				if (const auto& request = handler->handle(*event_info, m_client_id); nullptr != request) {
					try {
						m_communicator->send_request(request->pack());
					}
					catch (const std::exception& ex) {
						m_logger->log(std::string("Couldn't send event handler product request. Error: ") + ex.what());
					}
				}
			}
		}
		std::this_thread::sleep_for(std::chrono::seconds(EVENT_DETECTION_SLEEP_DURATION));
	}
}

void Client::execute_commands_loop()
{
	m_logger->log("Starting main loop of command fetching");
	while (true) {
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
	const auto request_to_make = handler->handle(command); // should usually be a ReturnProduct request
	if (const auto res = m_communicator->send_request(request_to_make->pack()); httplib::OK_200 != res.get_status()) {
		m_logger->log("Error sending ReturnProduct. Response: " + res.get_body());
		throw std::runtime_error("Couldn't ReturnProduct. Status code: " + std::to_string(res.get_status()));
	}
}

std::shared_ptr<BasicCommand> Client::get_command() const
{
	m_logger->log("Sending GetCommand request to server");

	BasicRequest request({RequestType::GetCommand, m_client_id});
	const auto res = m_communicator->send_request(request.pack());
	if (httplib::OK_200 != res.get_status()) {
		m_logger->log("Error sending GetCommand. Response: " + res.get_body());
		throw std::runtime_error("Couldn't GetCommand. Status code: " + std::to_string(res.get_status()));
	}
	auto response = GetCommandResponse().unpack(res);
	if (!response.has_command()) {
		return nullptr;
	}
	return response.get_command();
}
