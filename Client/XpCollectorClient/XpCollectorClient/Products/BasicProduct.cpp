#include "BasicProduct.h"
#include <chrono>
#include <format>

xp_collector::BasicProduct::BasicProduct(std::string product_id, const CommandType product_type)
	: IProduct(std::move(product_id))
	  , m_product_type(product_type)
{
}

json xp_collector::BasicProduct::serialize()
{
	const auto now = std::chrono::system_clock::now();
	const auto local_time = std::chrono::current_zone()->to_local(now);
	const std::string time_str = std::format("{:%Y-%m-%d %H:%M:%S}", local_time);
	return {
		{STATUS_FIELD_NAME, "success"},
		{"product_id", get_id()},
		{"product_type", to_string(m_product_type)},
		{"local_time", time_str}
	};
}
