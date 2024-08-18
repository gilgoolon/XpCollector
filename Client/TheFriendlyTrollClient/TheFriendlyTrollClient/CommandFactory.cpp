#include "CommandFactory.h"
#include "PopupCommand.h"

std::unique_ptr<BasicCommand> CommandFactory::create(const json& command)
{
    const auto command_id = command["command_id"];
    const auto command_type = from_string(command["command_type"]);
    auto basic = std::make_unique<BasicCommand>(command_id, command_type);
    switch (basic->get_command_type()) {
    case CommandType::Popup:
    case CommandType::PopupSpam:
        return std::make_unique<PopupCommand>(command_id, command_type, command["parameters"]["message"]);
    default:
        return std::move(basic);
    }
}
