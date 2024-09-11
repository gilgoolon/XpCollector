#pragma once
#include "EventHandlers/IEventHandler.h"

namespace xp_collector {

class LocalLogEventHandler :
    public IEventHandler
{
public:
	std::unique_ptr<IRequest> handle(std::shared_ptr<EventInfo> event_info, const std::string& client_id) override;
};

}
