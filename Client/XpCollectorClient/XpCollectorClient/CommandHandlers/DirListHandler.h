#pragma once
#include "ICommandHandler.h"

namespace xp_collector
{
class DirListHandler
	: public ICommandHandler
{
public:
	explicit DirListHandler(std::string client_id);

	std::unique_ptr<IRequest> handle(std::shared_ptr<BasicCommand>& command) override;
};
}
