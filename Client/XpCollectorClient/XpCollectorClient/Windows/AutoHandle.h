#pragma once
#include <Windows.h>

namespace windows
{
class AutoHandle
{
public:
	explicit AutoHandle(HANDLE handle);

	~AutoHandle();

	[[nodiscard]] HANDLE get() const;

private:
	HANDLE m_handle;
};
}
