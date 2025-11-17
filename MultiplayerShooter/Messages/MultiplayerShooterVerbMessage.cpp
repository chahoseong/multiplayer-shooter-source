#include "Messages/MultiplayerShooterVerbMessage.h"

FString FMultiplayerShooterVerbMessage::ToString() const
{
	FString Message;
	StaticStruct()->ExportText(
		Message,
		this,
		nullptr,
		nullptr,
		PPF_None,
		nullptr
	);
	return Message;
}
