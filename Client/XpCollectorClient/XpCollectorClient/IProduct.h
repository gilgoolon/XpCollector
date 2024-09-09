#pragma once
#include "JSON.h"

class IProduct
{
	virtual json serialize() = 0;
};
