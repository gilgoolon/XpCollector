#include "SystemInfoProduct.h"

#include "BasicProduct.h"
#include "Windows/WinUtils.h"

xp_collector::SystemInfoProduct::SystemInfoProduct(std::string product_id, const CommandType product_type,
                                                   json system_info)
	: BasicProduct(std::move(product_id), product_type)
	  , m_system_info(std::move(system_info))
{
}

json xp_collector::SystemInfoProduct::serialize()
{
	return m_system_info;
}
