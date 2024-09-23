#include "KeyLogCommand.h"
#include <iostream>

xp_collector::KeyLogCommand::KeyLogCommand(std::string command_id, const CommandType command_type,
                                           const unsigned int duration)
	: BasicCommand(std::move(command_id), command_type)
	  , m_duration(duration)
{
}

unsigned int xp_collector::KeyLogCommand::get_duration() const
{
	return m_duration;
}
