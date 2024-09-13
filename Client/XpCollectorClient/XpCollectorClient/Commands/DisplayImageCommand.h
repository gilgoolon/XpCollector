#pragma once
#include "BasicCommand.h"

namespace xp_collector
{
class DisplayImageCommand
	: public BasicCommand
{
public:
	DisplayImageCommand(std::string command_id, CommandType command_type, const std::string& image_buffer);

	[[nodiscard]] const std::string& get_image_buffer() const;

private:
	std::string m_image_buffer;
};
}
