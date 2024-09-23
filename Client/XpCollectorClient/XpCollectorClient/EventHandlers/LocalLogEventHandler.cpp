#include <iostream>
#include "EventHandlers/LocalLogEventHandler.h"
using namespace xp_collector;

std::unique_ptr<IRequest> LocalLogEventHandler::handle(const std::shared_ptr<EventInfo> event_info,
                                                       const std::string& client_id)
{
	std::cout << "Received event: " << event_info->pack().dump() << std::endl;
	return nullptr;
}
