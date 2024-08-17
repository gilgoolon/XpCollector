#include <httplib.h>

#include "HttpCommunicator.h"

HttpCommunicator::HttpCommunicator(std::string server_url)
	: m_http_client(server_url)
{
}

ResponseInfo HttpCommunicator::send_request(const RequestInfo& request)
{
	const auto body = request.get_body();
	const auto res = m_http_client.Post(request.get_resource(), body, "application/json");
	return ResponseInfo(res->status, res->body);
}
