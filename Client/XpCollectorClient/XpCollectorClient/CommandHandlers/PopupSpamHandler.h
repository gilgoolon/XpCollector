#pragma once
#include "ICommandHandler.h"

constexpr size_t POPUPS_COUNT = 20;

namespace xp_collector
{
class PopupSpamHandler :
	public ICommandHandler
{
public:
	explicit PopupSpamHandler(std::string client_id);

	std::unique_ptr<IRequest> handle(std::shared_ptr<BasicCommand>& command) override;
};
}
