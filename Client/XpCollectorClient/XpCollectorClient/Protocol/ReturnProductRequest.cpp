#include "Protocol/ReturnProductRequest.h"
#include "base64.hpp"

xp_collector::ReturnProductRequest::ReturnProductRequest(const RequestHeader& header, std::unique_ptr<IProduct> product)
	: BasicRequest(header)
	  , m_product(std::move(product))
{
}

xp_collector::RequestInfo xp_collector::ReturnProductRequest::pack()
{
	const auto encoded = base64::to_base64(m_product->serialize().dump());
	json body = {
		{"header", {{"request_type", to_string(m_header.request_type)}, {"client_id", m_header.client_id}}},
		{"content", {{"product_id", m_product->get_id()}, {"data", encoded}}}
	};
	return {"/", body};
}
