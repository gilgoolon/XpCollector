#pragma once
#include "BasicProduct.h"

namespace xp_collector {

class ScreenshotProduct :
    public BasicProduct
{
public:
    explicit ScreenshotProduct(std::string product_id, CommandType product_type, std::string&& bitmap_data);

    json serialize() override;

private:
    std::string m_bitmap_data;
};

}
