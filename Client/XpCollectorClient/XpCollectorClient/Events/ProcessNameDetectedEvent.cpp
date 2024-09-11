#include "ProcessNameDetectedEvent.h"
#include <WinUtils.h>
using namespace xp_collector;

ProcessNameDetectedEvent::ProcessNameDetectedEvent(std::vector<std::string> names)
	: m_names(std::move(names))
{
}

std::shared_ptr<EventInfo> ProcessNameDetectedEvent::is_detected()
{
	for (const auto& name : m_names) {
		if (windows::is_process_running(name)) {
			return std::make_shared<NamedFieldEventInfo<std::string>>(EventType::ProcessNameDetectedEvent, "name", name);
		}
	}
	return std::make_shared<EventInfo>(EventType::NotDetected);
}
