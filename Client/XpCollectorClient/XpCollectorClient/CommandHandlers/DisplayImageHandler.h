#pragma once
#include "ICommandHandler.h"

namespace xp_collector
{
class DisplayImageHandler
	: public ICommandHandler
{
public:
	explicit DisplayImageHandler(std::string client_id);

	std::unique_ptr<IRequest> handle(std::shared_ptr<BasicCommand>& command) override;
};
}
