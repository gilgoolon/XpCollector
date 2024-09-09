#pragma once
#include "CommandHandlers/ICommandHandler.h"

namespace xp_collector {

class CommandHandlerFactory
{
public:
	static std::unique_ptr<ICommandHandler> create(const BasicCommand& command, const std::string& command_id);
};

}
