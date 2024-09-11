#pragma once
#include <string>

namespace xp_collector
{
class IClientStorage
{
public:
	virtual void store(std::string name, std::string value) = 0;

	virtual bool has_field(std::string name) = 0;

	virtual std::string fetch(std::string name) = 0;
};
}
