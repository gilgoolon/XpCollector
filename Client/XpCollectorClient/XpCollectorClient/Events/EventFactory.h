#pragma once
#include <memory>
#include "Events/IEvent.h"
#include <Utils/JSON.h>

namespace xp_collector
{
class EventFactory
{
public:
	static std::unique_ptr<IEvent> create(json obj);
};
}
