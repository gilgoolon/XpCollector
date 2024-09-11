#include "AlwaysEvent.h"
using namespace xp_collector;

std::unique_ptr<EventInfo> xp_collector::AlwaysEvent::is_detected()
{
    return std::make_unique<EventInfo>(EventType::AlwaysEvent);
}
