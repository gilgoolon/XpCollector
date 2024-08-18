#pragma once
#include "ICommandHandler.h"

class PopupSpamHandler :
    public ICommandHandler
{
public:
	explicit PopupSpamHandler(const std::string& client_id);

	std::unique_ptr<IRequest> handle(std::shared_ptr<BasicCommand>& command) override;
};

