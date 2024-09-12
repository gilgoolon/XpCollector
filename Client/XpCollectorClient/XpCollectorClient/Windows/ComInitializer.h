#pragma once
#include <Windows.h>

namespace windows
{
class ComInitializer
{
public:
	explicit ComInitializer(DWORD init_type);

	~ComInitializer();
};
}
