#pragma once
#include "ICommandHandler.h"

namespace xp_collector
{
class UninstallHandler
	: public ICommandHandler
{
public:
	explicit UninstallHandler(const std::string& client_id);

	std::unique_ptr<IRequest> handle(std::shared_ptr<BasicCommand>& command) override;
};
}
