#pragma once
#include "Commands/BasicCommand.h"

namespace xp_collector
{
class PopupCommand :
	public BasicCommand
{
public:
	PopupCommand(std::string command_id, CommandType command_type, std::string message);

	std::string get_message();

private:
	std::string m_message;
};
}
