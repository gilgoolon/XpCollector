#include "PopupProduct.h"

xp_collector::PopupProduct::PopupProduct(std::string product_id, const bool is_success)
	: BasicProduct(std::move(product_id), CommandType::Popup)
	  , m_is_success(is_success)
{
}

json xp_collector::PopupProduct::serialize()
{
	json obj = BasicProduct::serialize();
	obj[CONTENT_FIELD_NAME] = {{"success", m_is_success}};
	return obj;
}
