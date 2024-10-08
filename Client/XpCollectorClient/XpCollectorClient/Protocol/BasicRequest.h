#pragma once
#include "IRequest.h"

namespace xp_collector
{
struct RequestHeader
{
	RequestType request_type;
	std::string client_id;
};


class BasicRequest
	: public IRequest
{
public:
	explicit BasicRequest(RequestHeader header);

	RequestInfo pack() override;

protected:
	RequestHeader m_header;
};
}
