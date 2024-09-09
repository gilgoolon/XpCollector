#include "Protocol/GetCommandResponse.h"
#include "Commands/CommandFactory.h"
using namespace xp_collector;

GetCommandResponse& xp_collector::GetCommandResponse::unpack(const ResponseInfo& response)
{
	BasicResponse::unpack(response);
	const auto content = response.get_body()["content"];
	m_exists = content["exists"];
	if (m_exists) {
		m_command = CommandFactory::create(content["command"]);
	}
	return *this;
}

bool xp_collector::GetCommandResponse::has_command() const
{
	return m_exists;
}

std::shared_ptr<BasicCommand> xp_collector::GetCommandResponse::get_command()
{
	return m_command;
}
