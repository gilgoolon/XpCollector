#pragma once
#include "RequestInfo.h"

class IRequest
{
public:
	virtual RequestInfo pack() = 0;
};

enum class RequestType {
	InstallClient,
	GetCommand,
	ReturnProduct
};

inline std::string to_string(RequestType val)
{
	switch (val)
	{
	case RequestType::InstallClient:   return "InstallClient";
	case RequestType::GetCommand:   return "GetCommand";
	case RequestType::ReturnProduct: return "WindowsReturnProduct";
	default: throw std::invalid_argument("Unmapped to_string enum in RequestType");
	}
}
