#pragma once
#include <filesystem>

#include "BasicCommand.h"

namespace xp_collector
{
class GetFileCommand
	: public BasicCommand
{
public:
	explicit GetFileCommand(std::string command_id, CommandType command_type, const std::string& path);

	std::filesystem::path get_path();

private:
	std::filesystem::path m_path;
};
}
