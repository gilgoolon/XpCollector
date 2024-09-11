#pragma once
#include "BasicResponse.h"

namespace xp_collector
{
class InstallClientResponse :
	public BasicResponse
{
public:
	InstallClientResponse& unpack(const ResponseInfo& response) override;

	std::string get_client_id() const;

private:
	std::string m_client_id;
};
}
