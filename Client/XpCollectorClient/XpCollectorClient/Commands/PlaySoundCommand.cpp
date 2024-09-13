#include "PlaySoundCommand.h"

#include <base64.hpp>

xp_collector::PlaySoundCommand::PlaySoundCommand(std::string command_id, const CommandType command_type,
                                                 const std::string& sound_buffer)
	: BasicCommand(std::move(command_id), command_type)
	  , m_sound_buffer(base64::from_base64(sound_buffer))
{
}

const std::string& xp_collector::PlaySoundCommand::get_sound_buffer() const
{
	return m_sound_buffer;
}
