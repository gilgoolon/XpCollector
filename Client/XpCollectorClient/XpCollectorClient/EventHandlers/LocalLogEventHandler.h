#pragma once
#include "EventHandlers/IEventHandler.h"

class LocalLogEventHandler :
    public IEventHandler
{
public:
    virtual std::unique_ptr<IRequest> handle(EventType event_type) override;
};

