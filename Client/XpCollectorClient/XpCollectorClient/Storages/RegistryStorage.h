#pragma once
#include "IStorage.h"

constexpr size_t MAX_FIELD_SIZE = 255;

namespace xp_collector
{
class RegistryStorage
	: public IClientStorage
{
public:
	explicit RegistryStorage(std::string key);

	void store(std::string name, std::string value) override;

	bool has_field(std::string name) override;

	std::string fetch(std::string name) override;

private:
	std::string m_key;
};
}
