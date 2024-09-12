#pragma once
#include <filesystem>

#include "Utils/JSON.h"

namespace windows
{
class FileInfo
{
public:
	explicit FileInfo(std::filesystem::path path);

	[[nodiscard]] json pack() const;

private:
	std::filesystem::path m_path;
	bool m_is_file;
	std::uintmax_t m_file_size;
	std::time_t m_last_write_time;
};
}
