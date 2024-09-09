#pragma once
#include "BasicCommand.h"

class PopupCommand :
    public BasicCommand
{
public:
	PopupCommand(std::string command_id, CommandType command_type, std::string message);

	std::string get_message();

private:
	std::string m_message;
};

