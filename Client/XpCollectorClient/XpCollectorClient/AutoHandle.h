#pragma once
#include <Windows.h>
#include <memory>

namespace xp_collector {
class AutoHandle {
public:
	explicit AutoHandle(HANDLE handle);

	~AutoHandle();

	HANDLE get() const;

private:
	HANDLE m_handle;
};
}
