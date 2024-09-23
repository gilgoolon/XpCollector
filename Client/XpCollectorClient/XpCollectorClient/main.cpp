#include <cstdlib>
#include <iostream>
#include <fstream>
#include "Configurator.h"
#include "resource.h"
#include "Windows/AutoHandle.h"
#include "Windows/WinUtils.h"
using namespace xp_collector;

constexpr size_t EXPECTED_ARGUMENTS_CONF_FILE = 2;
constexpr size_t EXPECTED_ARGUMENTS_CONF_RESOURCE = 1;
constexpr std::string_view ACTIVE_CLIENT_EVENT_NAME = "XpCollectorRunning";

namespace
{
std::unique_ptr<windows::AutoHandle> g_active_client_event = nullptr;
}

bool signal_running_event()
{
	if (windows::exists_event(ACTIVE_CLIENT_EVENT_NAME)) {
		return false;
	}
	g_active_client_event = windows::create_event(ACTIVE_CLIENT_EVENT_NAME, true);
	windows::signal_event(g_active_client_event->get());
	return true;
}

int main(const int argc, wchar_t** argv)
{
	if (!signal_running_event()) {
		std::cout << "XpCollector is already running on this machine! Exiting..." << std::endl;
		return EXIT_SUCCESS;
	}
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

	const std::unique_ptr<Client> client = parse(conf, argv[0]);
	client->run();
	return EXIT_SUCCESS;
}
