#pragma once
#include "ICommandHandler.h"

namespace xp_collector
{
class GetFileHandler
	: public ICommandHandler
{
public:
	explicit GetFileHandler(std::string client_id);

	std::unique_ptr<IRequest> handle(std::shared_ptr<BasicCommand>& command) override;
};
}
