#pragma once

#include <string>
#include <stdexcept>
#include <memory>
#include <JSON.h>

namespace xp_collector
{
enum class EventType
{
	NotDetected,
	AlwaysEvent,
	ProcessNameDetectedEvent
};

inline std::string to_string(const EventType val)
{
	switch (val)
	{
	case EventType::NotDetected: return "NotDetected";
	case EventType::AlwaysEvent: return "AlwaysEvent";
	case EventType::ProcessNameDetectedEvent: return "ProcessNameDetectedEvent";
	default:
		throw std::invalid_argument("Unmapped value in enum EventType to_string");
	}
}

inline EventType et_from_string(const std::string& val)
{
	if ("NotDetected" == val) return EventType::NotDetected;
	if ("AlwaysEvent" == val) return EventType::AlwaysEvent;
	if ("ProcessNameDetectedEvent" == val) return EventType::ProcessNameDetectedEvent;
	throw std::invalid_argument("Unmapped value in enum EventType to_string");
}

class EventInfo
{
public:
	explicit EventInfo(EventType type);

	virtual ~EventInfo() = default;

	EventType get_type() const;

	virtual json pack() const;

private:
	EventType m_type;
};

class IEvent
{
public:
	virtual std::unique_ptr<EventInfo> is_detected() = 0;

	virtual ~IEvent() = default;
};
}
