#include "AlwaysEvent.h"
using namespace xp_collector;

EventType xp_collector::AlwaysEvent::is_detected()
{
    return EventType::AlwaysEvent;
}
