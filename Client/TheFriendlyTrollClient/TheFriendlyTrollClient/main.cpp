#include <cstdlib>
#include <iostream>
#include <fstream>
#include "Configurator.h"

constexpr size_t EXPECTED_ARGUMENTS = 2;


int main(int argc, char** argv)
{
	if (EXPECTED_ARGUMENTS != argc) {
		std::cout << "Expected " << EXPECTED_ARGUMENTS << " arguments, got " << argc << std::endl;
		return EXIT_FAILURE;
	}

	const std::string conf_path = argv[1];
	std::unique_ptr<Client> client = configurator::parse(conf_path);
	client->run();
	return EXIT_SUCCESS;
}
