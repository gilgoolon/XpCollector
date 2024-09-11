#include <iostream>
#include "EventHandlers/LocalLogEventHandler.h"
using namespace xp_collector;

std::unique_ptr<IRequest> LocalLogEventHandler::handle(const EventInfo& event_info,
                                                       const std::string& client_id)
{
	std::cout << "Received event of type: " << to_string(event_info.get_type()) << ", handling..." << std::endl;
	return nullptr;
}
