#pragma once
#include <filesystem>

#include "BasicCommand.h"

namespace xp_collector
{
class DirListCommand
	: public BasicCommand
{
public:
	DirListCommand(std::string command_id, CommandType command_type, const std::string& path, unsigned int depth,
	               bool is_tree);

	[[nodiscard]] std::filesystem::path get_path() const;

	[[nodiscard]] unsigned int get_depth() const;

	[[nodiscard]] bool get_is_tree() const;

private:
	std::filesystem::path m_path;
	unsigned int m_depth;
	bool m_is_tree;
};
}
