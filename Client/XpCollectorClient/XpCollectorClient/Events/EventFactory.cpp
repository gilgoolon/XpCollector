#include "EventFactory.h"
#include "ProcessNameDetectedEvent.h"
using namespace xp_collector;

std::unique_ptr<IEvent> EventFactory::create(const json obj)
{
	const std::string name = obj["name"];
	const json params = obj["parameters"];
	switch (et_from_string(name)) {
	case EventType::ProcessNameDetectedEvent:
		return std::make_unique<ProcessNameDetectedEvent>(params["names"]);
	}
	throw std::runtime_error("Missing EventType in factory: " + name);
}
