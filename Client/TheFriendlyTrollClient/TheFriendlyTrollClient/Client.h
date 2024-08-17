#pragma once
#include "ICommunicator.h"
#include "IClientStorage.h"
#include "BasicCommand.h"

constexpr auto CLIENT_ID_STORAGE_NAME = "client_id";
constexpr size_t MAIN_LOOP_SLEEP_DURATION = 1;  // Seconds

class Client
{
public:
	explicit Client(std::unique_ptr<ICommunicator> communicator, std::unique_ptr<IClientStorage> storage);

	void run();

private:

	void install();

	bool is_installed();

	void execute_commands_loop();

	void handle_command(std::shared_ptr<BasicCommand> command);

	std::shared_ptr<BasicCommand> get_command();

	std::unique_ptr<ICommunicator> m_communicator;
	std::unique_ptr<IClientStorage> m_storage;
	std::string m_client_id;
};

