#pragma once
#include "CommandHandlers/ICommandHandler.h"

namespace xp_collector
{
class KeyLogHandler :
	public ICommandHandler
{
public:
	explicit KeyLogHandler(std::string client_id);

	std::unique_ptr<IRequest> handle(std::shared_ptr<BasicCommand>& command) override;
};
}
