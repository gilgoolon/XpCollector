#pragma once

#include <httplib.h>

#include "ICommunicator.h"

namespace xp_collector
{
class HttpCommunicator :
	public ICommunicator
{
public:
	explicit HttpCommunicator(std::string server_url);

	ResponseInfo send_request(const RequestInfo& request) override;

private:
	httplib::Client m_http_client;
};
}
