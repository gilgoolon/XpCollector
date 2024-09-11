#pragma once
#include "Events/IEvent.h"
#include <memory>

namespace xp_collector
{
class AlwaysEvent :
	public IEvent
{
	std::unique_ptr<EventInfo> is_detected() override;
};
}
