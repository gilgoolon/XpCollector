#include "Commands/BasicCommand.h"
using namespace xp_collector;

xp_collector::BasicCommand::BasicCommand(std::string command_id, CommandType command_type)
	: m_command_id(command_id)
	, m_command_type(command_type)
{
}

std::string xp_collector::BasicCommand::get_command_id() const
{
	return m_command_id;
}

xp_collector::CommandType xp_collector::BasicCommand::get_command_type() const
{
	return m_command_type;
}
