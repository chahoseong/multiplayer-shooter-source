#include "Messages/DustMessageLibrary.h"
#include "GameFramework/PlayerState.h"

APlayerState* UDustMessageLibrary::GetPlayerStateFromObject(UObject* Object)
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Object))
	{
		return PlayerController->PlayerState;
	}
	
	if (APlayerState* PlayerState = Cast<APlayerState>(Object))
	{
		return PlayerState;
	}
	
	if (const APawn* Pawn = Cast<APawn>(Object))
	{
		return Pawn->GetPlayerState();
	}
	
	return nullptr;
}

APlayerController* UDustMessageLibrary::GetPlayerControllerFromObject(UObject* Object)
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Object))
	{
		return PlayerController;
	}
	
	if (APlayerState* PlayerState = Cast<APlayerState>(Object))
	{
		return PlayerState->GetPlayerController();
	}
	
	if (const APawn* Pawn = Cast<APawn>(Object))
	{
		return Pawn->GetController<APlayerController>();
	}
	
	return nullptr;
}
