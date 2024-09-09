#pragma once
#include "EventHandlers/IEventHandler.h"

namespace xp_collector {

class LocalLogEventHandler :
    public IEventHandler
{
public:
    virtual std::unique_ptr<IRequest> handle(EventType event_type) override;
};

}
