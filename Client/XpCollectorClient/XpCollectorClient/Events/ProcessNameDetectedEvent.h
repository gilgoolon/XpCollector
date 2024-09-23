#pragma once
#include "Events/IEvent.h"
#include <vector>
#include <memory>

namespace xp_collector
{
class ProcessNameDetectedEvent
	: public IEvent
{
public:
	explicit ProcessNameDetectedEvent(std::vector<std::string> names);

	std::shared_ptr<EventInfo> is_detected() override;

private:
	std::vector<std::string> m_names;
};
}
