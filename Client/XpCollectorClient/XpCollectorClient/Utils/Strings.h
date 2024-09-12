#pragma once
#include <string>

namespace strings
{
std::wstring to_wstring(const std::string& input);

std::string to_string(const std::wstring& input);
}
