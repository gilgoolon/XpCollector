#pragma once
#include <string>

namespace xp_collector
{
class ILogger
{
public:
	virtual void log(const std::string& message) = 0;
};
}
