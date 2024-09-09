#include "ConsoleLogger.h"
#include "iostream"

void xp_collector::ConsoleLogger::log(const std::string& message)
{
	std::cout << message << std::endl;
}
