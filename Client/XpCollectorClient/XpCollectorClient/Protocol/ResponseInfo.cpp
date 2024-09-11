#include "ResponseInfo.h"

xp_collector::ResponseInfo::ResponseInfo(unsigned int status, std::string body)
	: m_status(status)
	  , m_body(body)
{
}

unsigned int xp_collector::ResponseInfo::get_status() const
{
	return m_status;
}

json xp_collector::ResponseInfo::get_body() const
{
	return json::parse(m_body);
}
