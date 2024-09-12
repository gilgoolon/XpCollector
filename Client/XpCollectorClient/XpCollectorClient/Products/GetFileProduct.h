#pragma once
#include "BasicProduct.h"

namespace xp_collector
{
class GetFileProduct
	: public BasicProduct
{
public:
	GetFileProduct(std::string product_id, CommandType product_type, std::filesystem::path path,
	               std::string contents);

	json serialize() override;

private:
	std::filesystem::path m_path;
	std::string m_contents;
};
}
