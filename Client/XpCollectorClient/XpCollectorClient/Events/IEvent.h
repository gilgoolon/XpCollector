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
	switch (val) {
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

	EventInfo(const EventInfo& other);

	virtual ~EventInfo() = default;

	[[nodiscard]] EventType get_type() const;

	[[nodiscard]] virtual json pack() const;

private:
	EventType m_type;
};

template <typename ValueType>
class NamedFieldEventInfo
	: public EventInfo
{
public:
	explicit NamedFieldEventInfo(const EventType type, std::string name, const ValueType& value)
		: EventInfo(type)
		  , m_name(std::move(name))
		  , m_value(std::move(value))
	{
	}

	NamedFieldEventInfo(const NamedFieldEventInfo& other)
		: EventInfo(other)
		  , m_name(other.m_name)
		  , m_value(other.m_value)
	{
	}

	[[nodiscard]] json pack() const override
	{
		json result = EventInfo::pack();
		result[m_name] = m_value;
		return result;
	}

	~NamedFieldEventInfo() override = default;

private:
	std::string m_name;
	ValueType m_value;
};

class IEvent
{
public:
	virtual std::shared_ptr<EventInfo> is_detected() = 0;

	virtual ~IEvent() = default;
};
}
