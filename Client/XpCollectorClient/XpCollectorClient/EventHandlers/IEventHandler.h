#pragma once
#include <memory>
#include "Protocol/IRequest.h"
#include "Events/IEvent.h"

namespace xp_collector
{
enum class EventHandlerType
{
	LocalLog,
	KeyLog
};

inline std::string to_string(const EventHandlerType type)
{
	switch (type)
	{
	case EventHandlerType::LocalLog:
		return "LocalLog";
	case EventHandlerType::KeyLog:
		return "KeyLog";
	default:
		throw std::invalid_argument("Unmapped EventHandlerType in factory");
	}
}

inline EventHandlerType eht_from_string(const std::string& value)
{
	if (value == "LocalLog")
	{
		return EventHandlerType::LocalLog;
	}
	if (value == "KeyLog")
	{
		return EventHandlerType::KeyLog;
	}
	throw std::invalid_argument("Unmapped EventHandlerType in factory");
}

class IEventHandler
{
public:
	virtual std::unique_ptr<IRequest> handle(const EventInfo& event_info, const std::string& client_id) = 0;

	virtual ~IEventHandler() = default;
};
}
