#include "Player/DustPlayerController.h"

#include "DustPlayerState.h"
#include "AbilitySystem/DustAbilitySystemComponent.h"

ADustPlayerController::ADustPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ADustPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ADustPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UDustAbilitySystemComponent* AbilitySystem = GetDustAbilitySystemComponent())
	{
		AbilitySystem->ProcessAbilityInput(DeltaTime, bGamePaused);
	}
	Super::PostProcessInput(DeltaTime, bGamePaused);
}

UDustAbilitySystemComponent* ADustPlayerController::GetDustAbilitySystemComponent() const
{
	const ADustPlayerState* DustPlayerState = GetPlayerState<ADustPlayerState>();
	return DustPlayerState ? DustPlayerState->GetDustAbilitySystemComponent() : nullptr;
}
