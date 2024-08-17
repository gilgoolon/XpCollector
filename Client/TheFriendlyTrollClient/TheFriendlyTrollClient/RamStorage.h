#pragma once
#include <unordered_map>

#include "IClientStorage.h"

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

