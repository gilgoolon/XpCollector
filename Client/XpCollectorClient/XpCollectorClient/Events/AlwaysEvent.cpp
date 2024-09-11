#include "AlwaysEvent.h"
using namespace xp_collector;

std::shared_ptr<EventInfo> AlwaysEvent::is_detected()
{
    return std::make_shared<EventInfo>(EventType::AlwaysEvent);
}
