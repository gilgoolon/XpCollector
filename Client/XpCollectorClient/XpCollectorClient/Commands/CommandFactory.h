#pragma once
#include <memory>
#include "Commands/BasicCommand.h"
#include "JSON.h"

namespace xp_collector {

class CommandFactory
{
public:
	static std::unique_ptr<BasicCommand> create(const json& command);
};

}
