#include "AutoHandle.h"

windows::AutoHandle::AutoHandle(const HANDLE handle)
	: m_handle(handle)
{
}

windows::AutoHandle::~AutoHandle()
{
	if (nullptr != m_handle && INVALID_HANDLE_VALUE != m_handle) {
		CloseHandle(m_handle);
	}
}

HANDLE windows::AutoHandle::get() const
{
	return m_handle;
}
