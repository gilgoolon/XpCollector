#include "FileInfo.h"

#include "Utils/Strings.h"

windows::FileInfo::FileInfo(std::filesystem::path path)
	: m_path(std::move(path))
{
	if ((m_is_file = is_regular_file(m_path))) {
		m_file_size = file_size(m_path);
	}
	else {
		m_file_size = 0;
	}
	const auto last_w_time = last_write_time(m_path);
	const auto last_w_system_time = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
		last_w_time - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now());
	m_last_write_time = std::chrono::system_clock::to_time_t(last_w_system_time);
}

json windows::FileInfo::pack() const
{
	std::tm local_time;
	localtime_s(&local_time, &m_last_write_time);
	std::stringstream ss;
	ss << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S");
	return {
		{"path", strings::to_string(m_path.wstring())},
		{"type", m_is_file ? "file" : "folder"},
		{"size", m_file_size},
		{"last_write_time", ss.str()}
	};
}
