#pragma once
#include <memory>
#include "Protocol/IRequest.h"
#include "Commands/BasicCommand.h"

namespace xp_collector
{
class ICommandHandler
{
public:
	explicit ICommandHandler(std::string client_id);

	virtual std::unique_ptr<IRequest> handle(std::shared_ptr<BasicCommand>& command) = 0;

protected:
	std::string m_client_id;
};
}
