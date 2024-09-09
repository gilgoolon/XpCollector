#include "RequestInfo.h"

RequestInfo::RequestInfo(std::string resource, json body)
	: m_resource(std::move(resource))
	, m_body(body)
{
}

std::string RequestInfo::get_resource() const
{
	return m_resource;
}

std::string RequestInfo::get_body() const
{
	return m_body.dump();
}
