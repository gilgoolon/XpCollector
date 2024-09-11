#pragma once
#include "EventHandlers/IEventHandler.h"

namespace xp_collector {

class LocalLogEventHandler :
    public IEventHandler
{
public:
    virtual std::unique_ptr<IRequest> handle(const EventInfo& event_info, const std::string& client_id) override;
};

}
