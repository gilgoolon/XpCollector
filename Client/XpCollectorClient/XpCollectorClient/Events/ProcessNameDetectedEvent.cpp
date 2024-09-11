#include "ProcessNameDetectedEvent.h"
#include <WinUtils.h>
using namespace xp_collector;

xp_collector::ProcessNameDetectedEvent::ProcessNameDetectedEvent(std::vector<std::string> names)
    : m_names(std::move(names))
{
}

std::unique_ptr<EventInfo> xp_collector::ProcessNameDetectedEvent::is_detected()
{
    for (const auto& name : m_names) {
        if (windows::is_process_running(name)) {
            return std::make_unique<EventInfo>(EventType::ProcessNameDetectedEvent);
        }
    }
    return std::make_unique<EventInfo>(EventType::NotDetected);
}
