#include "EventHandlerFactory.h"
#include "EventHandlers/LocalLogEventHandler.h"
#include "EventHandlers/KeyLogEventHandler.h"
using namespace xp_collector;

std::unique_ptr<IEventHandler> EventHandlerFactory::create(json object)
{
	const std::string name = object["name"];
	const json parameters = object["parameters"];
	switch (eht_from_string(name)) {
	case EventHandlerType::LocalLog:
		return std::make_unique<LocalLogEventHandler>();
	case EventHandlerType::KeyLog:
		const unsigned int duration = parameters["duration"];
		return std::make_unique<KeyLogEventHandler>(duration);
	}
	throw std::invalid_argument("Unmapped EventHandlerType in event handler factory");
}
