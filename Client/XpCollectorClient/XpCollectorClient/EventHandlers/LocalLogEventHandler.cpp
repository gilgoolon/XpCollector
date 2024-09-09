#include "LocalLogEventHandler.h"
#include <iostream>

std::unique_ptr<IRequest> LocalLogEventHandler::handle(EventType event_type)
{
	std::cout << "Received event of type: " << to_string(event_type) << ", handling..." << std::endl;
	return nullptr;
}
