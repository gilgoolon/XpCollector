#pragma once

#include <string>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

inline json json_get_or_default(const json& object, const std::string& key, json default_value)
{
	if (!object.contains(key) || object[key].is_null()) {
		return default_value;
	}
	return object[key];
}
