#pragma once
#include "EventHandlers/IEventHandler.h"

namespace xp_collector {

class KeyLogEventHandler :
    public IEventHandler
{
public:
    explicit KeyLogEventHandler(unsigned int duration);

    virtual std::unique_ptr<IRequest> handle(EventType event_type, const std::string& client_id) override;

private:
    unsigned int m_duration;
};

}
