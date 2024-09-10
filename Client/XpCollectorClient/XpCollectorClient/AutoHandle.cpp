#include "AutoHandle.h"

xp_collector::AutoHandle::AutoHandle(HANDLE handle)
	: m_handle(handle)
{
}

xp_collector::AutoHandle::~AutoHandle()
{
	if (nullptr != m_handle && INVALID_HANDLE_VALUE != m_handle) {
		CloseHandle(m_handle);
	}
}

HANDLE xp_collector::AutoHandle::get() const
{
	return m_handle;
}
