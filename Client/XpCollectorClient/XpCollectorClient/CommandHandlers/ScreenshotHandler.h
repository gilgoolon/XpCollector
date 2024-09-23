#pragma once
#include "CommandHandlers/ICommandHandler.h"

namespace xp_collector
{
class ScreenshotHandler :
	public ICommandHandler
{
public:
	explicit ScreenshotHandler(std::string client_id);

	std::unique_ptr<IRequest> handle(std::shared_ptr<BasicCommand>& command) override;
};
}
