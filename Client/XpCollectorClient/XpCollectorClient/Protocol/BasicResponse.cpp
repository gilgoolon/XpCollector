#include "BasicResponse.h"
using namespace xp_collector;

BasicResponse& xp_collector::BasicResponse::unpack(const ResponseInfo& response)
{
	m_header = { from_string(response.get_body()["header"]["status"]) };
	return *this;
}
