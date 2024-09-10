#include "LocalLogEventHandler.h"
#include <iostream>
using namespace xp_collector;

std::unique_ptr<IRequest> xp_collector::LocalLogEventHandler::handle(EventType event_type, const std::string& client_id)
{
	std::cout << "Received event of type: " << to_string(event_type) << ", handling..." << std::endl;
	return nullptr;
}
