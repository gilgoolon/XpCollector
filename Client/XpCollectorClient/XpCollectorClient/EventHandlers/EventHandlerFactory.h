#pragma once
#include <memory>
#include "IEventHandler.h"

namespace xp_collector {

class EventHandlerFactory
{
public:
	static std::unique_ptr<IEventHandler> create(json object);
};

}
