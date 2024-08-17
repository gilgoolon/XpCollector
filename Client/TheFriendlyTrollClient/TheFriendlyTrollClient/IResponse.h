#pragma once
#include "ResponseInfo.h"

enum class ResponseType {
	Success, Error
};

inline ResponseType from_string(std::string val)
{
	if ("Success" == val) return ResponseType::Success;
	if ("Error" == val) return ResponseType::Error;
	throw std::invalid_argument("Invalid string argument for ResponseType enum parsing");
}

class IResponse
{
public:
	virtual IResponse& unpack(const ResponseInfo& response) = 0;
};
