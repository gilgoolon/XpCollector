#pragma once
#include "Products/IProduct.h"
#include "Commands/BasicCommand.h"

constexpr auto CONTENT_FIELD_NAME = "content";
constexpr auto STATUS_FIELD_NAME = "status";

namespace xp_collector
{
class BasicProduct :
	public IProduct
{
public:
	explicit BasicProduct(std::string product_id, CommandType product_type);

	json serialize() override;

private:
	CommandType m_product_type;
};
}
