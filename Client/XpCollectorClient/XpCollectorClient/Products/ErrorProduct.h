#pragma once
#include "BasicProduct.h"
#include "IProduct.h"

namespace xp_collector
{
class ErrorProduct
	: public BasicProduct
{
public:
	explicit ErrorProduct(std::string id, CommandType product_type, std::string error_message);

	json serialize() override;

private:
	std::string m_error_message;
};
}
