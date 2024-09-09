#pragma once
#include "ICommandHandler.h"

class PopupHandler :
    public ICommandHandler
{
public:
    explicit PopupHandler(std::string client_id);

    virtual std::unique_ptr<IRequest> handle(std::shared_ptr<BasicCommand>& command) override;
};
