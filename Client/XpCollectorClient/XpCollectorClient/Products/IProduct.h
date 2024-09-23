#pragma once
#include "Utils/JSON.h"

namespace xp_collector
{
class IProduct
{
public:
	explicit IProduct(std::string id)
		: m_id(std::move(id))
	{
	}

	virtual ~IProduct() = default;

	virtual json serialize() = 0;

	[[nodiscard]] std::string get_id() const
	{
		return m_id;
	}

private:
	std::string m_id;
};
}
