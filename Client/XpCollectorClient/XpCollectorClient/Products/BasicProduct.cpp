#include "BasicProduct.h"
#include <chrono>
#include <format>

xp_collector::BasicProduct::BasicProduct(std::string product_id, CommandType product_type)
	: m_product_id(product_id)
	  , m_product_type(product_type)
{
}

json xp_collector::BasicProduct::serialize()
{
	const auto now = std::chrono::system_clock::now();
	const auto local_time = std::chrono::current_zone()->to_local(now);
	const std::string time_str = std::format("{:%Y-%m-%d %H:%M:%S}", local_time);
	return {
		{"product_id", m_product_id},
		{"product_type", to_string(m_product_type)},
		{"local_time", time_str}
	};
}
