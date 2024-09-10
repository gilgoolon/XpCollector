#include "EventHandlerFactory.h"
#include "LocalLogEventHandler.h"

std::unique_ptr<IEventHandler> xp_collector::EventHandlerFactory::create(json object)
{
    const std::string name = object["name"];
    const json parameters = object["parameters"];
    switch (eht_from_string(name)) {
    case EventHandlerType::LocalLog:
        return std::make_unique<LocalLogEventHandler>();
    }
    throw std::invalid_argument("Unmapped EventHandlerType in event handler factory");
}
