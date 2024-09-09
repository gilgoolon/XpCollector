#include "RamStorage.h"

void xp_collector::RamStorage::store(std::string name, std::string value)
{
    m_items[name] = value;
}

bool xp_collector::RamStorage::has_field(std::string name)
{
    return m_items.find(name) != m_items.end();
}

std::string xp_collector::RamStorage::fetch(std::string name)
{
    return m_items[name];
}
