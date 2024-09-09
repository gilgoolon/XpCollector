#pragma once
#include "IResponse.h"


struct ResponseHeader {
	ResponseType request_type;
};


class BasicResponse :
	public IResponse
{
public:
	BasicResponse& unpack(const ResponseInfo& response) override;
protected:
	ResponseHeader m_header;
};
