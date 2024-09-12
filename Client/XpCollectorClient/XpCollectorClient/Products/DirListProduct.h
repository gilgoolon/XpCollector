#pragma once
#include "BasicProduct.h"
#include "Windows/FileInfo.h"

namespace xp_collector
{
class DirListProduct
	: public BasicProduct
{
public:
	DirListProduct(std::string product_id, CommandType product_type,
	               std::vector<std::unique_ptr<windows::FileInfo>> paths);

	json serialize() override;

private:
	std::vector<std::unique_ptr<windows::FileInfo>> m_paths;
};
}
