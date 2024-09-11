#pragma once
#include "Protocol/BasicRequest.h"
#include "Protocol/ReturnProductRequest.h"
#include <Events/IEvent.h>

namespace xp_collector {

class ReturnEventProductRequest :
    public ReturnProductRequest
{
public:
    ReturnEventProductRequest(RequestHeader header, const std::string& product_id, EventInfo event_info, const json& data);

    RequestInfo pack() override;

private:
    EventInfo m_event_info;
};
}
