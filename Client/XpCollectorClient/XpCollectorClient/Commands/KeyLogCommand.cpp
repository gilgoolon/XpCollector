#include "KeyLogCommand.h"
#include <iostream>

xp_collector::KeyLogCommand::KeyLogCommand(std::string command_id, CommandType command_type, unsigned int duration)
	: BasicCommand(command_id, command_type)
	  , m_duration(duration)
{
}

unsigned int xp_collector::KeyLogCommand::get_duration() const
{
	return m_duration;
}
