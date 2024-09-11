#include "IEvent.h"
using namespace xp_collector;

EventInfo::EventInfo(const EventType type)
	: m_type(type)
{
}

EventInfo::EventInfo(const EventInfo& other)
	: m_type(other.m_type)
{
}

EventType EventInfo::get_type() const
{
	return m_type;
}

json EventInfo::pack() const
{
	return {{"type", to_string(m_type)}};
}
