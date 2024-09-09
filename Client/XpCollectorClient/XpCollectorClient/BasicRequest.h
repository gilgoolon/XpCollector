#pragma once
#include "IRequest.h"


struct RequestHeader {
	RequestType request_type;
	std::string client_id;
};


class BasicRequest
	: public IRequest
{
public:
	BasicRequest(RequestHeader header);

	RequestInfo pack() override;

protected:
	RequestHeader m_header;
};

