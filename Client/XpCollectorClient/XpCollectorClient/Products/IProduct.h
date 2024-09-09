#pragma once
#include "JSON.h"

namespace xp_collector {

class IProduct
{
	virtual json serialize() = 0;
};

}
