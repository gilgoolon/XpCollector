#pragma once
#include <memory>
#include "IRequest.h"
#include "BasicCommand.h"

class ICommandHandler
{
public:
	explicit ICommandHandler(const std::string& client_id);

	virtual std::unique_ptr<IRequest> handle(std::shared_ptr<BasicCommand>& command) = 0;

protected:
	std::string m_client_id;
};

