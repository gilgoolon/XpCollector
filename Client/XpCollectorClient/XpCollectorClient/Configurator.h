#pragma once
#include <string>
#include <memory>
#include "Client.h"

namespace xp_collector {
	std::unique_ptr<Client> parse(std::string conf_path);
}
