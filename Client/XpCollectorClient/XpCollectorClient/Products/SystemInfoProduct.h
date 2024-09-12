#pragma once
#include "BasicProduct.h"

namespace xp_collector
{
class SystemInfoProduct
	: public BasicProduct
{
public:
	explicit SystemInfoProduct(std::string product_id, CommandType product_type, json system_info);

	json serialize() override;

private:
	json m_system_info;
};
}
