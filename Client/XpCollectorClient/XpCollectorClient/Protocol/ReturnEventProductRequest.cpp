#include "ReturnEventProductRequest.h"
#include <base64.hpp>
using namespace xp_collector;

xp_collector::ReturnEventProductRequest::ReturnEventProductRequest(RequestHeader header, const std::string& product_id, EventType event_type, const json& data)
    : ReturnProductRequest(header, product_id, data)
    , m_event_type(event_type)
{
}

RequestInfo xp_collector::ReturnEventProductRequest::pack()
{
    const auto encoded = base64::to_base64(m_data.dump(PRODUCT_JSON_INDENT_SPACES_COUNT));
    json body = {
        {"header", {{"request_type", to_string(m_header.request_type)}, {"client_id", m_header.client_id}}},
        {"content", {{"product_id", m_product_id}, {"data", encoded}, {"event", to_string(m_event_type)}}},
    };
    return { "/", body };
}
