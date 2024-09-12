#include "GetFileCommand.h"

#include "Utils/Strings.h"

xp_collector::GetFileCommand::GetFileCommand(std::string command_id, const CommandType command_type,
                                             const std::string& path)
	: BasicCommand(std::move(command_id), command_type)
	  , m_path(strings::to_wstring(path))
{
}

std::filesystem::path xp_collector::GetFileCommand::get_path()
{
	return m_path;
}
