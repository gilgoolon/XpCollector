#pragma once
#include <type_traits>
#include <Windows.h>

template <typename WbemObjectType, std::enable_if_t<std::is_base_of_v<IUnknown, WbemObjectType>>* = nullptr>
class AutoWbemObject
{
public:
	explicit AutoWbemObject(WbemObjectType* object_ptr)
		: m_object_ptr(object_ptr)
	{
	}

	~AutoWbemObject()
	{
		if (nullptr != m_object_ptr) {
			m_object_ptr->Release();
		}
	}

	[[nodiscard]] WbemObjectType* get() const
	{
		return m_object_ptr;
	}

private:
	WbemObjectType* m_object_ptr;
};
