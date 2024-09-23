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

	void store(const std::string& name, const std::string& value) override;

	bool has_field(const std::string& name) override;

	std::string fetch(const std::string& name) override;

	void clear() override;

private:
	std::string m_key;
};
}
