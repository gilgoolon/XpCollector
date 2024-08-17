#include "PopupHandler.h"
#include "ReturnProductRequest.h"
#include "PopupProduct.h"

PopupHandler::PopupHandler(std::string client_id)
    : ICommandHandler(client_id)
{
}

std::unique_ptr<IRequest> PopupHandler::handle(std::shared_ptr<BasicCommand>& command)
{
    bool is_success = true;
    auto product = PopupProduct(command->get_command_id(), is_success);
    return std::make_unique<ReturnProductRequest>(
        RequestHeader{RequestType::ReturnProduct, m_client_id},
        command->get_command_id(),
        product.serialize()
    );
}
