#include <Windows.h>
#include <stdexcept>
#include "Strings.h"

std::wstring strings::to_wstring(const std::string& input)
{
	std::wstring out;
	if (!input.empty()) {
		const int len = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
		                                    input.c_str(), input.size(), nullptr, 0);
		if (len == 0) {
			throw std::runtime_error("Invalid character sequence.");
		}
		out.resize(len);
		MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
		                    input.c_str(), input.size(), out.data(), out.size());
	}
	return out;
}

std::string strings::to_string(const std::wstring& input)
{
	if (input.empty()) {
		return std::string{};
	}

	// Calculate the size needed for the resulting UTF-8 string
	const int utf8_length = WideCharToMultiByte(CP_UTF8, 0, input.c_str(), -1, nullptr, 0, nullptr, nullptr);
	if (utf8_length == 0) {
		throw std::runtime_error("Error in WideCharToMultiByte: Unable to calculate size.");
	}

	// Allocate space for the UTF-8 string
	std::string utf8_str(utf8_length - 1, '\0'); // Subtract 1 to exclude the null terminator

	// Perform the conversion
	if (const int result = WideCharToMultiByte(CP_UTF8, 0, input.c_str(), -1, utf8_str.data(), utf8_length, nullptr,
	                                           nullptr); result == 0) {
		throw std::runtime_error("Error in WideCharToMultiByte: Conversion failed.");
	}

	return utf8_str;
}
