#include "InstallClientResponse.h"

InstallClientResponse& InstallClientResponse::unpack(const ResponseInfo& response)
{
	BasicResponse::unpack(response);
	m_client_id = response.get_body()["content"]["client_id"];
	return *this;
}

std::string InstallClientResponse::get_client_id() const
{
	return m_client_id;
}
