#pragma once
#include <memory>
#include "Protocol/IRequest.h"
#include "Events/IEvent.h"
using namespace xp_collector;

namespace xp_collector{

enum class EventHandlerType {
	LocalLog,
	
};

inline std::string to_string(EventHandlerType type) {
	switch (type) {
	case EventHandlerType::LocalLog:
		return "LocalLog";
	default:
		throw std::invalid_argument("Unmapped EventHandlerType in factory");
	}
}

inline EventHandlerType eht_from_string(std::string value) {
	if (value == "LocalLog") {
		return EventHandlerType::LocalLog;
	}
	throw std::invalid_argument("Unmapped EventHandlerType in factory");
}

class IEventHandler
{
public:
	virtual std::unique_ptr<IRequest> handle(EventType event_type) = 0;
};

}
