#include "DirListProduct.h"

#include "Utils/Strings.h"

xp_collector::DirListProduct::DirListProduct(std::string product_id, const CommandType product_type,
                                             std::vector<std::unique_ptr<windows::FileInfo>> paths, const bool is_tree)
	: BasicProduct(std::move(product_id), product_type)
	  , m_paths(std::move(paths))
	  , m_is_tree(is_tree)
{
}

json xp_collector::DirListProduct::serialize()
{
	json result = BasicProduct::serialize();
	if (m_is_tree) {
		result[CONTENT_FIELD_NAME] = paths_to_json_tree(m_paths);
	}
	else {
		result[CONTENT_FIELD_NAME] = {};
		for (const auto& file_info : m_paths) {
			result[CONTENT_FIELD_NAME].push_back(file_info->pack());
		}
	}
	return result;
}

void xp_collector::DirListProduct::insert_path_in_json(json& j, const std::filesystem::path& path)
{
	json* current = &j; // Start at the root of the JSON tree

	for (const auto& part : path) {
		if (std::string part_str = strings::to_string(part.wstring()); current->contains(part_str)) {
			current = &(*current)[part_str];
		}
		else {
			// Check if it is the last part (file)
			if (part == path.filename()) {
				(*current)[part_str] = nullptr; // Insert as a file (nullptr)
			}
			else {
				// Insert a directory (empty object for now)
				(*current)[part_str] = json::object();
				current = &(*current)[part_str];
			}
		}
	}
}

json xp_collector::DirListProduct::flatten_root_path(const json& j)
{
	std::filesystem::path root;
	json cur_obj = j;
	while (1 == cur_obj.size()) {
		const auto item = cur_obj.items().begin();
		root /= strings::to_wstring(item.key());
		cur_obj = item.value();
	}
	if (root.empty()) {
		return j;
	}
	return {{strings::to_string(root.wstring()), cur_obj}};
}

// Main function to convert a vector of paths into a JSON tree
json xp_collector::DirListProduct::paths_to_json_tree(const std::vector<std::unique_ptr<windows::FileInfo>>& paths)
{
	json j;
	for (const auto& path : paths) {
		insert_path_in_json(j, path->get_path());
	}

	return flatten_root_path(j);
}
