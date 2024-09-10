#pragma once
#include "Protocol/BasicRequest.h"

constexpr size_t PRODUCT_JSON_INDENT_SPACES_COUNT = 4;

namespace xp_collector {

class ReturnProductRequest :
    public BasicRequest
{
public:
    ReturnProductRequest(RequestHeader header, const std::string& product_id, const json& data);

    RequestInfo pack() override;

protected:
    std::string m_product_id;
    json m_data;
};
}
