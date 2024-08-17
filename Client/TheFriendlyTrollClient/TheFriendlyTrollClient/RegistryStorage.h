#pragma once
#include "IClientStorage.h"

class RegistryStorage
	: public IClientStorage
{
	void store(std::string name, std::string value) override;
	std::string fetch(std::string name) override;
};

