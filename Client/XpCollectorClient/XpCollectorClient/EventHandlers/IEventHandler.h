#pragma once
#include <memory>
#include "Protocol/IRequest.h"
#include "Events/IEvent.h"

class IEventHandler
{
public:
	virtual std::unique_ptr<IRequest> handle(EventType event_type) = 0;
};
