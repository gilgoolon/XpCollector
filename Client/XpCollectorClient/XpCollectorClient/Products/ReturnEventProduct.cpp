#include "ReturnEventProduct.h"

xp_collector::ReturnEventProduct::ReturnEventProduct(std::unique_ptr<IProduct> product,
                                                     std::shared_ptr<EventInfo> event_info)
	: IProduct(product->get_id())
	  , m_product(std::move(product))
	  , m_event_info(std::move(event_info))
{
}

json xp_collector::ReturnEventProduct::serialize()
{
	json result = m_product->serialize();
	result["event"] = m_event_info->pack();
	return result;
}
