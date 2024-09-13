#pragma once
#define NOMINMAX
#include <Windows.h>
#include <memory>

namespace windows
{
struct HandleCloser
{
	void operator()(const HANDLE handle) const
	{
		if (nullptr != handle && INVALID_HANDLE_VALUE != handle) {
			CloseHandle(handle);
		}
	}
};

using AutoHandle = std::unique_ptr<std::remove_pointer_t<HANDLE>, HandleCloser>;
}
