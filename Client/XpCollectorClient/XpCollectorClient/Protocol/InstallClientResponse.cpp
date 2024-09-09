#include "InstallClientResponse.h"
using namespace xp_collector;

InstallClientResponse& xp_collector::InstallClientResponse::unpack(const ResponseInfo& response)
{
	BasicResponse::unpack(response);
	m_client_id = response.get_body()["content"]["client_id"];
	return *this;
}

std::string xp_collector::InstallClientResponse::get_client_id() const
{
	return m_client_id;
}
