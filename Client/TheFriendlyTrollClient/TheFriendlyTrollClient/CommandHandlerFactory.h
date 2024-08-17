#pragma once
#include "ICommandHandler.h"

class CommandHandlerFactory
{
public:
	static std::unique_ptr<ICommandHandler> create(const BasicCommand& command, const std::string& command_id);
};

