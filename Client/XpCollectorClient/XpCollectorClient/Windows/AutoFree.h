#pragma once
#include <cstdlib>
#include <memory>

namespace windows
{
struct MemoryFreer
{
	void operator()(void* ptr) const
	{
		if (nullptr != ptr) {
			free(ptr);
		}
	}
};

using AutoFree = std::unique_ptr<std::remove_pointer_t<void*>, MemoryFreer>;
}
