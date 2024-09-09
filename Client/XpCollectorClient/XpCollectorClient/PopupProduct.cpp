#include "PopupProduct.h"

PopupProduct::PopupProduct(std::string product_id, bool is_success)
    : BasicProduct(product_id, CommandType::Popup)
    , m_is_success(is_success)
{
}

json PopupProduct::serialize()
{
    json obj = BasicProduct::serialize();
    obj[CONTENT_FIELD_NAME] = { {"success", m_is_success} };
    return obj;
}
