#pragma once
#include "Events/IEvent.h"
#include <vector>

namespace xp_collector {

class ProcessNameDetectedEvent
	: public IEvent
{
public:
	explicit ProcessNameDetectedEvent(std::vector<std::string> names);

	virtual EventType is_detected() override;

private:
	std::vector<std::string> m_names;
};

}
