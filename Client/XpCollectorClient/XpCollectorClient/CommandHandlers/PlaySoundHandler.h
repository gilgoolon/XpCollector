#pragma once
#include "ICommandHandler.h"

namespace xp_collector
{
class PlaySoundHandler
	: public ICommandHandler
{
public:
	explicit PlaySoundHandler(const std::string& client_id);

	std::unique_ptr<IRequest> handle(std::shared_ptr<BasicCommand>& command) override;
};
}
