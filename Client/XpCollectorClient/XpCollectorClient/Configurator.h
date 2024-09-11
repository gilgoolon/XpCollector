#pragma once
#include <string>
#include <memory>
#include "Client.h"

namespace xp_collector
{
std::unique_ptr<Client> parse(std::string conf_path);
std::unordered_map<std::unique_ptr<IEvent>, std::vector<std::unique_ptr<IEventHandler>>> parse_events(json events);
}
