#pragma once
#include "ICommunicator.h"
#include "IClientStorage.h"
#include "BasicCommand.h"
#include "ILogger.h"
#include "IEvent.h"
#include "IEventHandler.h"

constexpr auto CLIENT_ID_STORAGE_NAME = "client_id";
constexpr size_t EXECUTE_COMMANDS_SLEEP_DURATION = 10;  // Seconds
constexpr size_t EVENT_DETECTION_SLEEP_DURATION = 5;  // Seconds
constexpr size_t INSTALLATION_RETRY_SLEEP_DURATION = 30;  // Seconds

class Client
{
public:
	explicit Client(
		std::unique_ptr<ICommunicator> communicator,
		std::unique_ptr<IClientStorage> storage,
		std::unordered_map<std::unique_ptr<IEvent>, std::vector<std::unique_ptr<IEventHandler>>>&& events,
		std::unique_ptr<ILogger> logger);

	void run();

private:

	void install();

	bool is_installed();

	void event_detection_loop(const std::unique_ptr<IEvent>& event_to_detect);

	void execute_commands_loop();

	void handle_command(std::shared_ptr<BasicCommand> command);

	std::shared_ptr<BasicCommand> get_command();

	std::unique_ptr<ICommunicator> m_communicator;
	std::unique_ptr<IClientStorage> m_storage;
	std::unique_ptr<ILogger> m_logger;
	std::unordered_map<std::unique_ptr<IEvent>, std::vector<std::unique_ptr<IEventHandler>>> m_events;
		std::string m_client_id;
};

