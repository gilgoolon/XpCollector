#pragma once
#include <memory>
#include "IRequest.h"
#include "IEvent.h"

class IEventHandler
{
public:
	virtual std::unique_ptr<IRequest> handle(EventType event_type) = 0;
};
