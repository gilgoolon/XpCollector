#include "ComInitializer.h"

#include <stdexcept>
#include <string>

windows::ComInitializer::ComInitializer(const DWORD init_type)
{
	if (const HRESULT res = CoInitializeEx(nullptr, init_type); FAILED(res)) {
		throw std::runtime_error("Failed to initialize COM library. Error: " + std::to_string(res));
	}
}

windows::ComInitializer::~ComInitializer()
{
	CoUninitialize();
}
