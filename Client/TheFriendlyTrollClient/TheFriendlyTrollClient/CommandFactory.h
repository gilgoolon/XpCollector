#pragma once
#include <memory>
#include "BasicCommand.h"
#include "JSON.h"

class CommandFactory
{
public:
	static std::unique_ptr<BasicCommand> create(const json& command);
};

