#pragma once
#include "EventHandlers/IEventHandler.h"

namespace xp_collector
{
class KeyLogEventHandler :
	public IEventHandler
{
public:
	explicit KeyLogEventHandler(unsigned int duration);

	std::unique_ptr<IRequest> handle(std::shared_ptr<EventInfo> event_info, const std::string& client_id) override;

private:
	unsigned int m_duration;
};
}
