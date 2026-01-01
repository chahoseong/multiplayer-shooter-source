#include "UI/Controllers/DustWidgetController.h"
#include "GameFramework/HUD.h"

UDustWidgetController::UDustWidgetController(const FObjectInitializer& ObjectInitializer)
{
}

void UDustWidgetController::InitializeController()
{
}

void UDustWidgetController::BroadcastInitialValues() const
{
}

UWorld* UDustWidgetController::GetWorld() const
{
	return GetOuter()->GetWorld();
}

APlayerController* UDustWidgetController::GetPlayerController() const
{
	if (const AHUD* HUD = Cast<AHUD>(GetOuter()))
	{
		return HUD->GetOwningPlayerController();
	}
	
	return nullptr;
}

ADustPlayerState* UDustWidgetController::GetDustPlayerState() const
{
	const APlayerController* PlayerController = GetPlayerController();
	return PlayerController->GetPlayerState<ADustPlayerState>();
}
