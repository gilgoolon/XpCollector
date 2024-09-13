#include "DirListCommand.h"

#include "Utils/Strings.h"

xp_collector::DirListCommand::DirListCommand(std::string command_id, const CommandType command_type,
                                             const std::string& path,
                                             const unsigned int depth)
	: BasicCommand(std::move(command_id), command_type)
	  , m_path(std::filesystem::path(strings::to_wstring(path)))
	  , m_depth(depth)
{
}

std::filesystem::path xp_collector::DirListCommand::get_path() const
{
	return m_path;
}

unsigned int xp_collector::DirListCommand::get_depth() const
{
	return m_depth;
}
