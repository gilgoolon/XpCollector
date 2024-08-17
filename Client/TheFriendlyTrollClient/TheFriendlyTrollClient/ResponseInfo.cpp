#include "ResponseInfo.h"

ResponseInfo::ResponseInfo(unsigned int status, std::string body)
	: m_status(status)
	, m_body(body)
{
}

unsigned int ResponseInfo::get_status() const
{
    return m_status;
}

json ResponseInfo::get_body() const
{
    return json::parse(m_body);
}
