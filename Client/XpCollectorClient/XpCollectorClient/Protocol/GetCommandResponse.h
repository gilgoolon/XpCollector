#pragma once
#include "Protocol/BasicResponse.h"
#include "Commands/BasicCommand.h"

class GetCommandResponse :
    public BasicResponse
{
public:
    GetCommandResponse& unpack(const ResponseInfo& response) override;

    bool has_command() const;

    std::shared_ptr<BasicCommand> get_command();

private:
    bool m_exists;
    std::shared_ptr<BasicCommand> m_command;
};

