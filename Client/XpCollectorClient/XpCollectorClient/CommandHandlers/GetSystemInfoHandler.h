#pragma once
#include "ICommandHandler.h"

namespace xp_collector
{
class GetSystemInfoHandler
	: public ICommandHandler
{
public:
	explicit GetSystemInfoHandler(std::string command_id);

	std::unique_ptr<IRequest> handle(std::shared_ptr<BasicCommand>& command) override;
};
}
