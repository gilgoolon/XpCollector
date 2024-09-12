#include "GetFileProduct.h"

#include "Utils/Strings.h"

xp_collector::GetFileProduct::GetFileProduct(std::string product_id, const CommandType product_type,
                                             std::filesystem::path path, std::string contents)
	: BasicProduct(std::move(product_id), product_type)
	  , m_path(std::move(path))
	  , m_contents(std::move(contents))
{
}

json xp_collector::GetFileProduct::serialize()
{
	json result = BasicProduct::serialize();
	result[CONTENT_FIELD_NAME] = {
		{"path", strings::to_string(m_path.wstring())},
		{"contents", std::move(m_contents)}
	};
	return result;
}
