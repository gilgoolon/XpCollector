#pragma once
#include <filesystem>

#include "BasicCommand.h"

namespace xp_collector
{
class DirListCommand
	: public BasicCommand
{
public:
	DirListCommand(std::string command_id, CommandType command_type, const std::string& path, unsigned int depth);

	[[nodiscard]] std::filesystem::path get_path() const;

	[[nodiscard]] unsigned int get_depth() const;

private:
	std::filesystem::path m_path;
	unsigned int m_depth;
};
}
