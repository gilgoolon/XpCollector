#pragma once
#include <string>
#include <Windows.h>

namespace win_utils {
	bool do_popup(const std::string& window_name, const std::string& text, const DWORD flags);

	void do_popups(const size_t count, const std::string& window_name, const std::string& text, const DWORD flags);

	std::string take_screenshot();
}
