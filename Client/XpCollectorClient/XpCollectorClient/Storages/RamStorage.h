#pragma once
#include <unordered_map>

#include "IStorage.h"

namespace xp_collector {

class RamStorage :
    public IClientStorage
{
public:
    void store(std::string name, std::string value) override;
    bool has_field(std::string name) override;
    std::string fetch(std::string name) override;
private:
    std::unordered_map<std::string, std::string> m_items;
};

}
