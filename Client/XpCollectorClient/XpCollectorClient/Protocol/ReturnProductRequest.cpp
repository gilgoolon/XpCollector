#include "Protocol/ReturnProductRequest.h"
#include "base64.hpp"

xp_collector::ReturnProductRequest::ReturnProductRequest(RequestHeader header, const std::string& product_id,
                                                         const json& data)
	: BasicRequest(header)
	  , m_product_id(product_id)
	  , m_data(data)
{
}

xp_collector::RequestInfo xp_collector::ReturnProductRequest::pack()
{
	const auto encoded = base64::to_base64(m_data.dump(PRODUCT_JSON_INDENT_SPACES_COUNT));
	json body = {
		{"header", {{"request_type", to_string(m_header.request_type)}, {"client_id", m_header.client_id}}},
		{"content", {{"product_id", m_product_id}, {"data", encoded}}}
	};
	return {"/", body};
}
