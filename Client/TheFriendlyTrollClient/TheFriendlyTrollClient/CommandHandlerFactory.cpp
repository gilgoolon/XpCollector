#include "CommandHandlerFactory.h"
#include "PopupHandler.h"

std::unique_ptr<ICommandHandler> CommandHandlerFactory::create(const BasicCommand& command, const std::string& command_id)
{
	switch (command.get_command_type())
	{
	case CommandType::Popup: return std::make_unique<PopupHandler>(command_id);
	default: throw std::invalid_argument("Unhandled command type in CommandHandlerFactory");
	}
}
