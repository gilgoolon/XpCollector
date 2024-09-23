#include "Uuid.h"
#include <random>

static constexpr std::string_view HEX_CHARS = "0123456789abcdef";

std::string uuid::generate_uuid(const size_t length)
{
	std::string result(length, '\0');
	for (size_t i = 0; i < length; i++) {
		result[i] = HEX_CHARS[rand_int(0, HEX_CHARS.length() - 1)];
	}
	return result;
}

int uuid::rand_int(const int min, const int max)
{
	thread_local std::mt19937 generator;
	std::uniform_int_distribution distribution(min, max);
	return distribution(generator);
}
