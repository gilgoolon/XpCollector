#include <stdexcept>
#include <chrono>
#include <thread>
#include <iostream>

#include <httplib.h>

#include "Client.h"
#include "BasicRequest.h"
#include "InstallClientResponse.h"

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

	std::cout << m_client_id << std::endl;

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
	m_storage->store(CLIENT_ID_STORAGE_NAME, m_client_id); // should make an install request here
}

bool Client::is_installed()
{
	return m_storage->has_field(CLIENT_ID_STORAGE_NAME);
}

void Client::execute_commands_loop()
{
	while (true) {
		get_command();

		std::this_thread::sleep_for(std::chrono::seconds(MAIN_LOOP_SLEEP_DURATION));
	}
}

void Client::get_command()
{
	std::cout << "Processing Commands..." << std::endl;
}
