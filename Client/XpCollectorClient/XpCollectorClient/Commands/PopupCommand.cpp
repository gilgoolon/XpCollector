#include "PopupCommand.h"

xp_collector::PopupCommand::PopupCommand(std::string command_id, const CommandType command_type, std::string message)
	: BasicCommand(std::move(command_id), command_type)
	  , m_message(std::move(message))
{
}

std::string xp_collector::PopupCommand::get_message()
{
	return m_message;
}
