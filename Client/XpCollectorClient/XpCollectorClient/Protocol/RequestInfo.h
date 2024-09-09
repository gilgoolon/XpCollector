#pragma once

#include "JSON.h"

namespace xp_collector {

class RequestInfo
{
public:
	RequestInfo(std::string resource, json body);

	std::string get_resource() const;

	std::string get_body() const;
private:
	std::string m_resource; // I.E. index.html
	json m_body;
};

}
