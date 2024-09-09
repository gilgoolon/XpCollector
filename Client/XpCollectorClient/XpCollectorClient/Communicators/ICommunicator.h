#pragma once
#include "Protocol/RequestInfo.h"
#include "Protocol/ResponseInfo.h"

namespace xp_collector {

class ICommunicator
{
public:
	virtual ResponseInfo send_request(const RequestInfo& request) = 0;
};

}
