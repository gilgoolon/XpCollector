#include <base64.hpp>

#include "ScreenshotProduct.h"

ScreenshotProduct::ScreenshotProduct(std::string product_id, CommandType product_type, std::string&& bitmap_data)
    : BasicProduct(product_id, product_type)
    , m_bitmap_data(std::move(bitmap_data))
{
}

json ScreenshotProduct::serialize()
{
    json obj = BasicProduct::serialize();
    obj["bitmap_data"] = base64::to_base64(m_bitmap_data);
    return obj;
}
