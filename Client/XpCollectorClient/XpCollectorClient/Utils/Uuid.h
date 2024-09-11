#pragma once
#include <string>

namespace uuid
{
std::string generate_uuid(size_t length = 32);

int rand_int(int min, int max);
}
