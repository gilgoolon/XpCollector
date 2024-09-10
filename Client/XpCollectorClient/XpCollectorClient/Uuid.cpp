#include "Uuid.h"
#include <random>
#include <random>
#include <string_view>

static const std::string_view hex_chars = "0123456789abcdef";

std::string uuid::generate_uuid(size_t length)
{
    std::string result(length, '\0');
    for (size_t i = 0; i < length; i++) {
        result += hex_chars[std::rand() % hex_chars.length()];
    }
    return result;
}
