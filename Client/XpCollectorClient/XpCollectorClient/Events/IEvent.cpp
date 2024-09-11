#include "IEvent.h"
using namespace xp_collector;

EventInfo::EventInfo(const EventType type)
	: m_type(type)
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
