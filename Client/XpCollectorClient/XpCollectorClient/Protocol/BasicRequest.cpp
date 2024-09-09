#include "BasicRequest.h"
using namespace xp_collector;

xp_collector::BasicRequest::BasicRequest(RequestHeader header)
    : m_header(header)
{
}

RequestInfo xp_collector::BasicRequest::pack()
{
    json body = {
        {"header", {{"request_type", to_string(m_header.request_type)}, {"client_id", m_header.client_id}}}
    };
    return {"/", body};
}
