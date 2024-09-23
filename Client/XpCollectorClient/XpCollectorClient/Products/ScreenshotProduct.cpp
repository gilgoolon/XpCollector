#include <base64.hpp>

#include "ScreenshotProduct.h"

xp_collector::ScreenshotProduct::ScreenshotProduct(std::string product_id, const CommandType product_type,
                                                   std::string&& bitmap_data)
	: BasicProduct(std::move(product_id), product_type)
	  , m_bitmap_data(std::move(bitmap_data))
{
}

json xp_collector::ScreenshotProduct::serialize()
{
	json obj = BasicProduct::serialize();
	obj["bitmap_data"] = base64::to_base64(m_bitmap_data);
	return obj;
}
