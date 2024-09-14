#pragma once
#include <unordered_map>

#include "IStorage.h"

namespace xp_collector
{
class RamStorage :
	public IClientStorage
{
public:
	void store(const std::string& name, const std::string& value) override;
	bool has_field(const std::string& name) override;
	std::string fetch(const std::string& name) override;
	void clear() override;

private:
	std::unordered_map<std::string, std::string> m_items;
};
}
