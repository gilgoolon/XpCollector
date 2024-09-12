#include "ErrorProduct.h"

xp_collector::ErrorProduct::ErrorProduct(std::string id, const CommandType product_type, std::string error_message)
	: BasicProduct(std::move(id), product_type)
	  , m_error_message(std::move(error_message))
{
}

json xp_collector::ErrorProduct::serialize()
{
	json result = BasicProduct::serialize();
	result[STATUS_FIELD_NAME] = "error";
	result[CONTENT_FIELD_NAME] = {{"error_message", m_error_message}};
	return result;
}
