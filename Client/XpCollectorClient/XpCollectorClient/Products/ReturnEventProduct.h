#pragma once
#include "IProduct.h"
#include "Events/IEvent.h"

namespace xp_collector
{
class ReturnEventProduct
	: public IProduct
{
public:
	ReturnEventProduct(std::unique_ptr<IProduct> product, std::shared_ptr<EventInfo> event_info);

	json serialize() override;

private:
	std::unique_ptr<IProduct> m_product;
	std::shared_ptr<EventInfo> m_event_info;
};
}
