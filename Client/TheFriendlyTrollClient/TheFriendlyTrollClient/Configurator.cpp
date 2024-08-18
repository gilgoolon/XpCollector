#include <fstream>

#include "Configurator.h"
#include "nlohmann/json.hpp"
#include "HttpCommunicator.h"
#include "RamStorage.h"
#include "ConsoleLogger.h"
#include "FileLogger.h"
#include "RegistryStorage.h"

using json = nlohmann::json;

std::unique_ptr<Client> configurator::parse(std::string conf_path)
{
	std::ifstream conf_file(conf_path);
	std::string contents;
	json conf = json::parse(conf_file);
	std::string server_url = conf["server_url"];
	const bool debug = conf["debug"];
	std::unique_ptr<ILogger> logger = nullptr;
	if (debug) {
		logger = std::make_unique<ConsoleLogger>();
	}
	else {
		logger = std::make_unique<FileLogger>(conf["log_folder"]);
	}
	return std::make_unique<Client>(
		std::make_unique<HttpCommunicator>(server_url),
		std::make_unique<RegistryStorage>(conf["registry_storage_key"]),
		std::move(logger)
	);
}
