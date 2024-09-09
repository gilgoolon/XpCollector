#include "ConsoleLogger.h"
#include "iostream"

void ConsoleLogger::log(const std::string& message)
{
	std::cout << message << std::endl;
}
