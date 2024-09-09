#pragma once
#include "Protocol/RequestInfo.h"
#include "Protocol/ResponseInfo.h"


class ICommunicator
{
public:
	virtual ResponseInfo send_request(const RequestInfo& request) = 0;
};

