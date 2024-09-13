#include "DisplayImageCommand.h"

#include <base64.hpp>

xp_collector::DisplayImageCommand::DisplayImageCommand(std::string command_id, const CommandType command_type,
                                                       const std::string& image_buffer)
	: BasicCommand(std::move(command_id), command_type)
	  , m_image_buffer(base64::from_base64(image_buffer))
{
}

const std::string& xp_collector::DisplayImageCommand::get_image_buffer() const
{
	return m_image_buffer;
}
