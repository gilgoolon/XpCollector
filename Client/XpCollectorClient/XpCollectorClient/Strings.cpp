#include <Windows.h>
#include <stdexcept>
#include "Strings.h"

std::wstring strings::to_wstring(const std::string& input)
{
	std::wstring out;
	if (input.length() > 0) {
		int len = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
			input.c_str(), input.size(), NULL, 0);
		if (len == 0) {
			throw std::runtime_error("Invalid character sequence.");
		}
		out.resize(len);
		MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
			input.c_str(), input.size(), &out[0], out.size());
	}
	return out;
}
