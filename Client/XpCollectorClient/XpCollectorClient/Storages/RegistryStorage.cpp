#define NOMINMAX
#include <Windows.h>

#include "RegistryStorage.h"
#include <stdexcept>
#include <utility>

xp_collector::RegistryStorage::RegistryStorage(std::string key)
	: m_key(std::move(key))
{
}

void xp_collector::RegistryStorage::store(const std::string& name, const std::string& value)
{
	HKEY h_key;
	DWORD error = RegCreateKeyExA(HKEY_CURRENT_USER, m_key.c_str(), 0, nullptr, 0, KEY_WRITE, nullptr, &h_key, nullptr);
	if (ERROR_SUCCESS != error) {
		throw std::runtime_error("Failed opening registry key for writing. Error code: " + std::to_string(error));
	}
	error = RegSetValueExA(h_key, name.c_str(), 0, REG_SZ, reinterpret_cast<LPCBYTE>(value.c_str()),
	                       static_cast<DWORD>(value.size()));
	if (ERROR_SUCCESS != error) {
		RegCloseKey(h_key);
		throw std::runtime_error("Failed storing registry value. Error code: " + std::to_string(error));
	}
	RegCloseKey(h_key);
}

bool xp_collector::RegistryStorage::has_field(const std::string& name)
{
	try {
		fetch(name);
		return true;
	}
	catch (const std::invalid_argument&) {
		return false;
	}
}

std::string xp_collector::RegistryStorage::fetch(const std::string& name)
{
	HKEY h_key;
	DWORD result_size = MAX_FIELD_SIZE;
	char result[MAX_FIELD_SIZE];
	DWORD dw_type = REG_SZ; // Specifies the type of data we expect

	// Open the registry key
	DWORD error = RegOpenKeyExA(HKEY_CURRENT_USER, m_key.c_str(), 0, KEY_READ, &h_key);
	if (ERROR_SUCCESS != error) {
		if (ERROR_FILE_NOT_FOUND == error) {
			throw std::invalid_argument("Failed opening registry key for reading, program key doesn't exist");
		}
		throw std::runtime_error("Failed opening registry key for reading. Error code: " + std::to_string(error));
	}
	error = RegQueryValueExA(h_key, name.c_str(), nullptr, &dw_type, reinterpret_cast<LPBYTE>(result), &result_size);
	if (ERROR_SUCCESS != error) {
		RegCloseKey(h_key);
		if (ERROR_FILE_NOT_FOUND == error) {
			throw std::invalid_argument("Failed reading registry value, field with name '" + name + "' doesn't exist");
		}
		throw std::runtime_error("Failed reading registry value. Error code: " + std::to_string(error));
	}
	RegCloseKey(h_key);
	return {result, result_size - 1}; // remove null terminator
}

void xp_collector::RegistryStorage::clear()
{
	if (const auto result = RegDeleteTreeA(HKEY_CURRENT_USER, m_key.c_str()); ERROR_SUCCESS != result) {
		throw std::runtime_error("Failed to clear registry storage. Error: " + std::to_string(result));
	}
}
