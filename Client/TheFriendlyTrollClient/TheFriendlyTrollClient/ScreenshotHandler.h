#pragma once
#include <Windows.h>

#include "ICommandHandler.h"


class ScreenshotHandler :
    public ICommandHandler
{
public:
    explicit ScreenshotHandler(std::string client_id);

    virtual std::unique_ptr<IRequest> handle(std::shared_ptr<BasicCommand>& command) override;
};

