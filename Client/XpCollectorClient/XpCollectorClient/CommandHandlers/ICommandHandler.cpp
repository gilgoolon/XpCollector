#include "ICommandHandler.h"

xp_collector::ICommandHandler::ICommandHandler(std::string client_id)
	: m_client_id(std::move(client_id))
{
}
