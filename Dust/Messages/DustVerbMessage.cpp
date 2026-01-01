#include "Messages/DustVerbMessage.h"

FString FDustVerbMessage::ToString() const
{
	FString HumanReadableMessage;
	StaticStruct()->ExportText(
		HumanReadableMessage,
		this,
		nullptr,
		nullptr,
		PPF_None,
		nullptr
	);
	return HumanReadableMessage;
}
