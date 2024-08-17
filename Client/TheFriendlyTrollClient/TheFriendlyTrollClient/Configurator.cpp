#include <fstream>

#include "Configurator.h"
#include "nlohmann/json.hpp"
#include "HttpCommunicator.h"
#include "RamStorage.h"

using json = nlohmann::json;

std::unique_ptr<Client> configurator::parse(std::string conf_path)
{
	std::ifstream conf_file(conf_path);
	std::string contents;
	json conf = json::parse(conf_file);
	std::string server_url = conf["server_url"];
	
	return std::make_unique<Client>(
		std::make_unique<HttpCommunicator>(server_url),
		std::make_unique<RamStorage>()
	);
}
