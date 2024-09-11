#pragma once
#include "Communicators/ICommunicator.h"
#include "Storages/IStorage.h"
#include "Commands/BasicCommand.h"
#include "Loggers/ILogger.h"
#include "Events/IEvent.h"
#include "EventHandlers/IEventHandler.h"

namespace xp_collector
{
constexpr auto CLIENT_ID_STORAGE_NAME = "client_id";
constexpr size_t EXECUTE_COMMANDS_SLEEP_DURATION = 10; // Seconds
constexpr size_t EVENT_DETECTION_SLEEP_DURATION = 5; // Seconds
constexpr size_t INSTALLATION_RETRY_SLEEP_DURATION = 30; // Seconds

class Client
{
public:
	explicit Client(
		std::unique_ptr<ICommunicator> communicator,
		std::unique_ptr<IClientStorage> storage,
		std::unordered_map<std::unique_ptr<IEvent>, std::vector<std::unique_ptr<IEventHandler>>>&& events,
		std::unique_ptr<ILogger> logger);

	[[noreturn]] void run();

private:
	void install() const;

	[[nodiscard]] bool is_installed() const;

	void event_detection_loop(const std::unique_ptr<IEvent>& event_to_detect,
	                          const std::vector<std::unique_ptr<IEventHandler>>& handlers) const;

	[[noreturn]] void execute_commands_loop();

	void handle_command(std::shared_ptr<BasicCommand> command) const;

	std::shared_ptr<BasicCommand> get_command() const;

	std::unique_ptr<ICommunicator> m_communicator;
	std::unique_ptr<IClientStorage> m_storage;
	std::unique_ptr<ILogger> m_logger;
	std::unordered_map<std::unique_ptr<IEvent>, std::vector<std::unique_ptr<IEventHandler>>> m_events;
	std::string m_client_id;
};
}
