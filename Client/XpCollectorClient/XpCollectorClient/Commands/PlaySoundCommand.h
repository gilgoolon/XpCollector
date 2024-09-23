#pragma once
#include "BasicCommand.h"

namespace xp_collector
{
class PlaySoundCommand
	: public BasicCommand
{
public:
	PlaySoundCommand(std::string command_id, CommandType command_type, const std::string& sound_buffer);

	[[nodiscard]] const std::string& get_sound_buffer() const;

private:
	std::string m_sound_buffer;
};
}
