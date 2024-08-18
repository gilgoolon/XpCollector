#pragma once
#include <string>

namespace win_utils {
	bool do_popup(const std::string& window_name, const std::string& text, const DWORD flags);

	std::string take_screenshot();
}
