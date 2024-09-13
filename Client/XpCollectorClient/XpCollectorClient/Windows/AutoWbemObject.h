#pragma once
#include <type_traits>
#define NOMINMAX
#include <Windows.h>

namespace windows
{
template <typename WbemObjectType, std::enable_if_t<std::is_base_of_v<IUnknown, WbemObjectType>>* = nullptr>
struct WbemObjectReleaser
{
	void operator()(WbemObjectType* object)
	{
		if (nullptr != object) {
			object->Release();
		}
	}
};

template <typename WbemObjectType, std::enable_if_t<std::is_base_of_v<IUnknown, WbemObjectType>>* = nullptr>
using AutoWbemObject = std::unique_ptr<std::remove_pointer_t<WbemObjectType*>, WbemObjectReleaser<WbemObjectType>>;
}
