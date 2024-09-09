#include "RequestInfo.h"

xp_collector::RequestInfo::RequestInfo(std::string resource, json body)
	: m_resource(std::move(resource))
	, m_body(body)
{
}

std::string xp_collector::RequestInfo::get_resource() const
{
	return m_resource;
}

std::string xp_collector::RequestInfo::get_body() const
{
	return m_body.dump();
}
