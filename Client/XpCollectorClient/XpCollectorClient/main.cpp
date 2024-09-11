#include <cstdlib>
#include <iostream>
#include <fstream>
#include "Configurator.h"
#include "resource.h"
#include "Windows/WinUtils.h"
using namespace xp_collector;

constexpr size_t EXPECTED_ARGUMENTS_CONF_FILE = 2;
constexpr size_t EXPECTED_ARGUMENTS_CONF_RESOURCE = 1;


int main(const int argc, char** argv)
{
	json conf;
	if (EXPECTED_ARGUMENTS_CONF_FILE == argc) {
		conf = json::parse(windows::read_file(std::filesystem::path(argv[1])));
	}
	else if (EXPECTED_ARGUMENTS_CONF_RESOURCE == argc) {
		conf = json::parse(windows::get_binary_resource(CLIENT_CONFIGURATION));
	}
	else {
		std::cout << "Expected " << EXPECTED_ARGUMENTS_CONF_FILE << " or " << EXPECTED_ARGUMENTS_CONF_RESOURCE <<
			" arguments, got " << argc << std::endl;
		return EXIT_FAILURE;
	}

	const std::unique_ptr<Client> client = parse(conf);
	client->run();
	return EXIT_SUCCESS;
}
