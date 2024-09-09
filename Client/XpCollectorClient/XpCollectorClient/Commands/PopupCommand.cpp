#include "PopupCommand.h"

PopupCommand::PopupCommand(std::string command_id, CommandType command_type, std::string message)
	: BasicCommand(command_id, command_type)
	, m_message(message)
{
}

std::string PopupCommand::get_message()
{
	return m_message;
}
