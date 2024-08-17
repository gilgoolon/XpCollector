#include <stdexcept>
#include <chrono>
#include <thread>
#include <iostream>
#include <thread>

#include <httplib.h>

#include "Client.h"
#include "BasicRequest.h"
#include "InstallClientResponse.h"
#include "GetCommandResponse.h"
#include "PopupCommand.h"
#include "CommandHandlerFactory.h"

Client::Client(std::unique_ptr<ICommunicator> communicator, std::unique_ptr<IClientStorage> storage)
	: m_communicator(std::move(communicator))
	, m_storage(std::move(storage))
{
}

void Client::run()
{
	if (!is_installed()) {
		install();
	}

	std::cout << "Client ID: " << m_client_id << std::endl;

	execute_commands_loop();
}

void Client::install()
{
	BasicRequest request({ RequestType::InstallClient, "" });

	const ResponseInfo res = m_communicator->send_request(request.pack());
	if (httplib::OK_200 != res.get_status()) {
		std::cout << res.get_body() << std::endl;
		throw std::runtime_error("Couldn't install properly. Status code: " + std::to_string(res.get_status()));
	}
	m_client_id = InstallClientResponse().unpack(res).get_client_id();
	m_storage->store(CLIENT_ID_STORAGE_NAME, m_client_id);
}

bool Client::is_installed()
{
	return m_storage->has_field(CLIENT_ID_STORAGE_NAME);
}

void Client::execute_commands_loop()
{
	while (true) {
		auto command = get_command();
		if (nullptr == command) {
			std::cout << "No new commands..." << std::endl;
		}
		else {
			std::cout << "New command: " << command->get_command_id() + " " << to_string(command->get_command_type()) << std::endl;
			std::thread handling_thread(&Client::handle_command, this, std::move(command));
			handling_thread.detach();
		}
		std::this_thread::sleep_for(std::chrono::seconds(MAIN_LOOP_SLEEP_DURATION));
	}
}

void Client::handle_command(std::shared_ptr<BasicCommand> command)
{
	std::cout << "Handling '" << to_string(command->get_command_type()) << "' command with id " << command->get_command_id() << " in a different thread!" << std::endl;
	const auto handler = CommandHandlerFactory::create(*command, m_client_id);

	const auto request_to_make = handler->handle(command); // should usually be a ReturnProduct request
	const auto res = m_communicator->send_request(request_to_make->pack()); // expect 200 from ReturnProduct
	if (httplib::OK_200 != res.get_status()) {
		std::cout << res.get_body() << std::endl;
		throw std::runtime_error("Couldn't request from handler properly. Status code: " + std::to_string(res.get_status()));
	}
}

std::shared_ptr<BasicCommand> Client::get_command()
{
	BasicRequest request({ RequestType::GetCommand, m_client_id });

	const auto res = m_communicator->send_request(request.pack());
	if (httplib::OK_200 != res.get_status()) {
		std::cout << res.get_body() << std::endl;
		throw std::runtime_error("Couldn't GetCommand. Status code: " + std::to_string(res.get_status()));
	}
	auto response = GetCommandResponse();
	response.unpack(res);
	if (!response.has_command()) {
		return nullptr;
	}
	return response.get_command();
}
