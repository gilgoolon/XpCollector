#include "DirListCommand.h"

#include "Utils/Strings.h"

xp_collector::DirListCommand::DirListCommand(std::string command_id, const CommandType command_type,
                                             const std::string& path,
                                             const unsigned int depth, const bool is_tree)
	: BasicCommand(std::move(command_id), command_type)
	  , m_path(std::filesystem::path(strings::to_wstring(path)))
	  , m_depth(depth)
	  , m_is_tree(is_tree)
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

bool xp_collector::DirListCommand::get_is_tree() const
{
	return m_is_tree;
}
