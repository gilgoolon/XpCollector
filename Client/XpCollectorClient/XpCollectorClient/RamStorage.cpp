#include "RamStorage.h"

void RamStorage::store(std::string name, std::string value)
{
    m_items[name] = value;
}

bool RamStorage::has_field(std::string name)
{
    return m_items.find(name) != m_items.end();
}

std::string RamStorage::fetch(std::string name)
{
    return m_items[name];
}
