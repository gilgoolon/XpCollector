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
	               std::vector<std::unique_ptr<windows::FileInfo>> paths,
	               bool is_tree);

	json serialize() override;

private:
	static json paths_to_json_tree(const std::vector<std::unique_ptr<windows::FileInfo>>& paths);

	static void insert_path_in_json(json& j, const std::filesystem::path& path);

	static json flatten_root_path(const json& j);

	std::vector<std::unique_ptr<windows::FileInfo>> m_paths;
	bool m_is_tree;
};
}
