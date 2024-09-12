#include "BasicRequest.h"
using namespace xp_collector;

BasicRequest::BasicRequest(RequestHeader header)
	: m_header(std::move(header))
{
}

RequestInfo BasicRequest::pack()
{
	json body = {
		{"header", {{"request_type", to_string(m_header.request_type)}, {"client_id", m_header.client_id}}}
	};
	return {"/", body};
}
