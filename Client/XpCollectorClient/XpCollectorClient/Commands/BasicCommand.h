#pragma once
#include <string>
#include <stdexcept>

namespace xp_collector {

enum class CommandType {
	Popup,
	PopupSpam,
	Screenshot
};

inline std::string to_string(CommandType val)
{
	switch (val) {
	case CommandType::Popup: return "Popup";
	case CommandType::PopupSpam: return "PopupSpam";
	case CommandType::Screenshot: return "Screenshot";
	default: throw std::invalid_argument("Unmapped to_string enum in CommandType");
	}
}

inline CommandType ct_from_string(const std::string& val)
{
	if ("Popup" == val) return CommandType::Popup;
	if ("PopupSpam" == val) return CommandType::PopupSpam;
	if ("Screenshot" == val) return CommandType::Screenshot;
	throw std::invalid_argument("Invalid argument '" + val + "' when trying to parse CommandType enum");
}

class BasicCommand
{
public:
	BasicCommand(std::string command_id, CommandType command_type);

	std::string get_command_id() const;

	CommandType get_command_type() const;

protected:
	std::string m_command_id;
	CommandType m_command_type;
};

}
