#pragma once
#include "Products/IProduct.h"
#include "Protocol/BasicRequest.h"

constexpr size_t PRODUCT_JSON_INDENT_SPACES_COUNT = 4;

namespace xp_collector
{
class ReturnProductRequest :
	public BasicRequest
{
public:
	ReturnProductRequest(const RequestHeader& header, std::unique_ptr<IProduct> product);

	RequestInfo pack() override;

protected:
	std::unique_ptr<IProduct> m_product;
};
}
