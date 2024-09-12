#include "DirListProduct.h"

xp_collector::DirListProduct::DirListProduct(std::string product_id, const CommandType product_type,
                                             std::vector<std::unique_ptr<windows::FileInfo>> paths)
	: BasicProduct(std::move(product_id), product_type)
	  , m_paths(std::move(paths))
{
}

json xp_collector::DirListProduct::serialize()
{
	json result = BasicProduct::serialize();
	result[CONTENT_FIELD_NAME]["dirlist"] = {};
	for (const auto& file_info : m_paths) {
		result[CONTENT_FIELD_NAME]["dirlist"].push_back(file_info->pack());
	}
	return result;
}
