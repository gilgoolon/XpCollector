#include "DisplayImageHandler.h"

#include "Commands/DisplayImageCommand.h"
#include "Products/BasicProduct.h"
#include "Protocol/ReturnProductRequest.h"
#include "Windows/WinUtils.h"

xp_collector::DisplayImageHandler::DisplayImageHandler(std::string client_id)
	: ICommandHandler(std::move(client_id))
{
}

std::unique_ptr<xp_collector::IRequest> xp_collector::DisplayImageHandler::handle(
	std::shared_ptr<BasicCommand>& command)
{
	const auto display_image_command = std::static_pointer_cast<DisplayImageCommand>(command);
	const auto image = windows::pgn_buffer_to_image(display_image_command->get_image_buffer());
	windows::display_image_window(image);
	return std::make_unique<ReturnProductRequest>(
		RequestHeader{RequestType::ReturnEventProduct, m_client_id},
		std::make_unique<BasicProduct>(command->get_command_id(), command->get_command_type())
	);
}
