#pragma once
#include <string>

namespace xp_collector
{
class IClientStorage
{
public:
	virtual ~IClientStorage() = default;
	virtual void store(const std::string& name, const std::string& value) = 0;

	virtual bool has_field(const std::string& name) = 0;

	virtual std::string fetch(const std::string& name) = 0;

	virtual void clear() = 0;
};
}
