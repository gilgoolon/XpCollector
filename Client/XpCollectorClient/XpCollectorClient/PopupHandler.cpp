#include "PopupHandler.h"
#include "ReturnProductRequest.h"
#include "PopupProduct.h"
#include <Windows.h>
#include "PopupCommand.h"
#include "WinUtils.h"

PopupHandler::PopupHandler(std::string client_id)
    : ICommandHandler(client_id)
{
}

std::unique_ptr<IRequest> PopupHandler::handle(std::shared_ptr<BasicCommand>& command)
{
    std::shared_ptr<PopupCommand> popup_command = std::static_pointer_cast<PopupCommand>(command);
    const bool is_success = win_utils::do_popup("Popup", popup_command->get_message(), MB_ICONERROR);
    return std::make_unique<ReturnProductRequest>(
        RequestHeader{RequestType::ReturnProduct, m_client_id},
        command->get_command_id(),
        PopupProduct(command->get_command_id(), is_success).serialize()
    );
}