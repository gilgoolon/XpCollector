#include "RamStorage.h"

void xp_collector::RamStorage::store(const std::string& name, const std::string& value)
{
	m_items[name] = value;
}

bool xp_collector::RamStorage::has_field(const std::string& name)
{
	return m_items.contains(name);
}

std::string xp_collector::RamStorage::fetch(const std::string& name)
{
	return m_items[name];
}

void xp_collector::RamStorage::clear()
{
	m_items.clear();
}
