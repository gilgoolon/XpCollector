#pragma once

#include <string>
#include <stdexcept>

enum class EventType {
	NotDetected,
	AlwaysEvent
};

inline std::string to_string(EventType val)
{
	switch (val) {
	case EventType::NotDetected: return "NotDetected";
	case EventType::AlwaysEvent: return "AlwaysEvent";
	default:
		throw std::invalid_argument("Unmapped value in enum EventType to_string");
	}
}

inline EventType et_from_string(const std::string& val)
{
	if ("NotDetected" == val) return EventType::NotDetected;
	if ("AlwaysEvent" == val) return EventType::AlwaysEvent;
	throw std::invalid_argument("Unmapped value in enum EventType to_string");
}

class IEvent
{
public:
	virtual EventType is_detected() = 0;
};
