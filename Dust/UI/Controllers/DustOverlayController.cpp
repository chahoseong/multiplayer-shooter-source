#include "UI/Controllers/DustOverlayController.h"
#include "AbilitySystem/Attributes/DustHealthSet.h"
#include "Player/DustPlayerState.h"

UDustOverlayController::UDustOverlayController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UDustOverlayController::InitializeController()
{
	Super::InitializeController();

	if (const ADustPlayerState* DustPlayerState = GetDustPlayerState())
	{
		UAbilitySystemComponent* AbilitySystem = DustPlayerState->GetAbilitySystemComponent();
		
		// Health
		AbilitySystem->GetGameplayAttributeValueChangeDelegate(UDustHealthSet::GetHealthAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data){
			OnHealthChanged.Broadcast(Data.NewValue);
		});
		
		// Max Health
		AbilitySystem->GetGameplayAttributeValueChangeDelegate(UDustHealthSet::GetMaxHealthAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data){
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		});
	}
}

void UDustOverlayController::BroadcastInitialValues() const
{
	if (const ADustPlayerState* DustPlayerState = GetDustPlayerState())
	{
		const UDustHealthSet* HealthSet = DustPlayerState->GetHealthSet();
		OnHealthChanged.Broadcast(HealthSet->GetHealth());
		OnMaxHealthChanged.Broadcast(HealthSet->GetMaxHealth());
	}
}
