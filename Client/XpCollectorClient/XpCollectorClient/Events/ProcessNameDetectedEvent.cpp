#include "ProcessNameDetectedEvent.h"
#include <WinUtils.h>
using namespace xp_collector;

xp_collector::ProcessNameDetectedEvent::ProcessNameDetectedEvent(std::vector<std::string> names)
    : m_names(std::move(names))
{
}

EventType xp_collector::ProcessNameDetectedEvent::is_detected()
{
    for (const auto& name : m_names) {
        if (windows::is_process_running(name)) {
            return EventType::ProcessNameDetectedEvent;
        }
    }
    return EventType::NotDetected;
}
