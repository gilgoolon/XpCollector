#include <fstream>

#include "Configurator.h"
#include "nlohmann/json.hpp"
#include "Communicators/HttpCommunicator.h"
#include "Storages/RamStorage.h"
#include "Loggers/ConsoleLogger.h"
#include "Loggers/FileLogger.h"
#include "Storages/RegistryStorage.h"
#include "Events/AlwaysEvent.h"
#include "EventHandlers/LocalLogEventHandler.h"

using json = nlohmann::json;

std::unique_ptr<Client> configurator::parse(std::string conf_path)
{
	std::ifstream conf_file(conf_path);
	std::string contents;
	json conf = json::parse(conf_file);
	std::string server_url = conf["server_url"];
	const bool debug = conf["debug"];
	
	std::unordered_map<std::unique_ptr<IEvent>, std::vector<std::unique_ptr<IEventHandler>>> events;
	
	std::vector<std::unique_ptr<IEventHandler>> always_event_handlers;
	always_event_handlers.push_back(std::make_unique<LocalLogEventHandler>());
	events.insert_or_assign(std::make_unique<AlwaysEvent>(), std::move(always_event_handlers));

	std::unique_ptr<ILogger> logger = nullptr;
	if (debug) {
		logger = std::make_unique<ConsoleLogger>();
	}
	else {
		logger = std::make_unique<FileLogger>(conf["log_folder"]);
	}
	return std::make_unique<Client>(
		std::make_unique<HttpCommunicator>(server_url),
		std::make_unique<RegistryStorage>(conf["registry_storage_key"]),
		std::move(events),
		std::move(logger)
	);
}
