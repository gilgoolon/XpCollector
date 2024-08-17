#include "PopupHandler.h"
#include "ReturnProductRequest.h"
#include "PopupProduct.h"
#include <Windows.h>
#include "PopupCommand.h"

PopupHandler::PopupHandler(std::string client_id)
    : ICommandHandler(client_id)
{
}

std::unique_ptr<IRequest> PopupHandler::handle(std::shared_ptr<BasicCommand>& command)
{
    std::shared_ptr<PopupCommand> popup_command = std::static_pointer_cast<PopupCommand>(command);
    bool is_success = do_popup(popup_command->get_message());
    auto product = PopupProduct(command->get_command_id(), is_success);
    return std::make_unique<ReturnProductRequest>(
        RequestHeader{RequestType::ReturnProduct, m_client_id},
        command->get_command_id(),
        product.serialize()
    );
}

bool PopupHandler::do_popup(std::string message)
{
    const auto result = MessageBoxA(GetForegroundWindow(), message.c_str(), "Popup", MB_ICONERROR);
    if (!result) {
        return false;
    }
    return true;
}
