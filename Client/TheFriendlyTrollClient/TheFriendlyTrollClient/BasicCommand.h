#pragma once
#include <string>
#include <stdexcept>

enum class CommandType {
	Popup
};

inline std::string to_string(CommandType val)
{
	switch (val) {
	case CommandType::Popup: return "Popup";
	default: throw std::invalid_argument("Unmapped to_string enum in CommandType");
	}
}

inline CommandType from_string(const std::string& val)
{
	if ("Popup" == val) return CommandType::Popup;
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

