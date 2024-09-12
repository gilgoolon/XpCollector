#pragma once
#include "Protocol/RequestInfo.h"

namespace xp_collector
{
class IRequest
{
public:
	virtual ~IRequest() = default;

	virtual RequestInfo pack() = 0;
};

enum class RequestType
{
	InstallClient,
	GetCommand,
	ReturnProduct,
	ReturnEventProduct
};

inline std::string to_string(const RequestType val)
{
	switch (val) {
	case RequestType::InstallClient: return "InstallClient";
	case RequestType::GetCommand: return "GetCommand";
	case RequestType::ReturnProduct: return "ReturnProduct";
	case RequestType::ReturnEventProduct: return "ReturnEventProduct";
	default: throw std::invalid_argument("Unmapped to_string enum in RequestType");
	}
}
}
