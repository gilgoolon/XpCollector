#pragma once
#include "RequestInfo.h"
#include "ResponseInfo.h"


class ICommunicator
{
public:
	virtual ResponseInfo send_request(const RequestInfo& request) = 0;
};

