#pragma once
#include "Products/BasicProduct.h"

namespace xp_collector
{
class KeyLogProduct :
	public BasicProduct
{
public:
	explicit KeyLogProduct(std::string product_id, CommandType product_type, std::string result);

	json serialize() override;

private:
	const std::string m_result;
};
}
