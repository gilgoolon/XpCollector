#pragma once
#include <string>

class ILogger
{
public:
	virtual void log(const std::string& message) = 0;
};

