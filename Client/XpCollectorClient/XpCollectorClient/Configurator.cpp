#include <fstream>
#include <vector>

#include "Configurator.h"
#include "nlohmann/json.hpp"
#include "Communicators/HttpCommunicator.h"
#include "Storages/RamStorage.h"
#include "Loggers/ConsoleLogger.h"
#include "Loggers/FileLogger.h"
#include "Storages/RegistryStorage.h"
#include "Events/AlwaysEvent.h"
#include "EventHandlers/LocalLogEventHandler.h"
#include "Events/ProcessNameDetectedEvent.h"
#include "EventHandlers/EventHandlerFactory.h"
#include "Events/EventFactory.h"
using json = nlohmann::json;

std::unique_ptr<xp_collector::Client> xp_collector::parse(std::string conf_path)
{
	std::ifstream conf_file(conf_path);
	std::string contents;
	json conf = json::parse(conf_file);
	std::string server_url = conf["server_url"];
	const bool debug = conf["debug"];
	
	std::unordered_map<std::unique_ptr<IEvent>, std::vector<std::unique_ptr<IEventHandler>>> events = parse_events(conf["events"]);

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

std::unordered_map<std::unique_ptr<IEvent>, std::vector<std::unique_ptr<IEventHandler>>> xp_collector::parse_events(json events)
{
	std::unordered_map<std::unique_ptr<IEvent>, std::vector<std::unique_ptr<IEventHandler>>> result;
	for (const json& object : events) {
		const json event_obj = object["event"];
		std::vector <std::unique_ptr<IEventHandler>> handlers;
		for (const json& handler_obj : object["handlers"]) {
			handlers.push_back(std::move(EventHandlerFactory::create(handler_obj)));
		}
		result.insert_or_assign(EventFactory::create(event_obj), std::move(handlers));
	}
	return result;
}
