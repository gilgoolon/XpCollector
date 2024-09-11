#pragma once
#include "Utils/JSON.h"

namespace xp_collector
{
class IProduct
{
public:
	virtual ~IProduct() = default;

	virtual json serialize() = 0;
};
}
