#include "GetCommandResponse.h"
#include "CommandFactory.h"

GetCommandResponse& GetCommandResponse::unpack(const ResponseInfo& response)
{
	BasicResponse::unpack(response);
	const auto content = response.get_body()["content"];
	m_exists = content["exists"];
	if (m_exists) {
		m_command = CommandFactory::create(content["command"]);
	}
	return *this;
}

bool GetCommandResponse::has_command() const
{
	return m_exists;
}

std::shared_ptr<BasicCommand> GetCommandResponse::get_command()
{
	return m_command;
}
