#include "Configurator.h"

#include <fstream>
#include <vector>

#include "Communicators/HttpCommunicator.h"
#include "EventHandlers/EventHandlerFactory.h"
#include "Events/AlwaysEvent.h"
#include "Events/EventFactory.h"
#include "Loggers/ConsoleLogger.h"
#include "Loggers/FileLogger.h"
#include "Storages/RamStorage.h"
#include "Storages/RegistryStorage.h"
using namespace xp_collector;

std::unique_ptr<Client> xp_collector::parse(const json& conf)
{
	std::string server_url = conf["server_url"];
	const bool debug = conf["debug"];

	std::unordered_map<std::unique_ptr<IEvent>, std::vector<std::unique_ptr<IEventHandler>>> events = parse_events(
		conf["events"]);

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

std::unordered_map<std::unique_ptr<IEvent>, std::vector<std::unique_ptr<IEventHandler>>>
xp_collector::parse_events(json events)
{
	std::unordered_map<std::unique_ptr<IEvent>, std::vector<std::unique_ptr<IEventHandler>>> result;
	for (const json& object : events) {
		const json event_obj = object["event"];
		std::vector<std::unique_ptr<IEventHandler>> handlers;
		for (const json& handler_obj : object["handlers"]) {
			handlers.push_back(std::move(EventHandlerFactory::create(handler_obj)));
		}
		result.insert_or_assign(EventFactory::create(event_obj), std::move(handlers));
	}
	return result;
}
