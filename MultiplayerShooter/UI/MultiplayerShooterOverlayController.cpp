#include "UI/MultiplayerShooterOverlayController.h"
#include "AbilitySystem/Attributes/MultiplayerShooterHealthSet.h"
#include "Player/MultiplayerShooterPlayerState.h"
#include "UI/MultiplayerShooterHUD.h"
#include "UI/MultiplayerShooterUserWidget.h"

UMultiplayerShooterOverlayController* UMultiplayerShooterOverlayController::GetOverlayController(APlayerController* InPlayerController)
{
	if (const AMultiplayerShooterHUD* HUD = InPlayerController->GetHUD<AMultiplayerShooterHUD>())
	{
		return HUD->GetOverlayController();
	}
	return nullptr;
}

void UMultiplayerShooterOverlayController::Initialize(APlayerController* NewPlayerController)
{
	Super::Initialize(NewPlayerController);

	AMultiplayerShooterPlayerState* PlayerState =
		GetPlayerState<AMultiplayerShooterPlayerState>();

	if (!IsValid(PlayerState))
	{
		return;
	}

	if (UAbilitySystemComponent* AbilitySystem = PlayerState->GetAbilitySystemComponent())
	{
		// Health
		FOnGameplayAttributeValueChange& HealthChangeDelegate =
			AbilitySystem->GetGameplayAttributeValueChangeDelegate(
				UMultiplayerShooterHealthSet::GetHealthAttribute()
			);
		HealthChangeDelegate.AddLambda(
			[this](const FOnAttributeChangeData& Data){
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);

		// Max Health
		FOnGameplayAttributeValueChange& MaxHealthChangeDelegate =
			AbilitySystem->GetGameplayAttributeValueChangeDelegate(
				UMultiplayerShooterHealthSet::GetMaxHealthAttribute()
			);
		MaxHealthChangeDelegate.AddLambda(
			[this](const FOnAttributeChangeData& Data){
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);
	}
}

void UMultiplayerShooterOverlayController::BroadcastInitialAttributes()
{
	const AMultiplayerShooterPlayerState* PlayerState =
		GetPlayerState<AMultiplayerShooterPlayerState>();
	
	if (IsValid(PlayerState))
	{
		const UMultiplayerShooterHealthSet* HealthSet =
			PlayerState->GetHealthSet();

		if (IsValid(HealthSet))
		{
			OnHealthChanged.Broadcast(HealthSet->GetHealth());
			OnMaxHealthChanged.Broadcast(HealthSet->GetMaxHealth());
		}
	}
}

void UMultiplayerShooterOverlayController::SetCrosshair(TSubclassOf<UMultiplayerShooterUserWidget> CrosshairWidgetClass)
{
	ActiveCrosshairWidget = CreateWidget<UMultiplayerShooterUserWidget>(
		PlayerController.Get(),
		CrosshairWidgetClass
	);
	ActiveCrosshairWidget->SetWidgetController(this);
	ActiveCrosshairWidget->AddToViewport();
}

void UMultiplayerShooterOverlayController::UpdateCrosshairSpread(float BaseSpreadAngle, float SpreadAngleMultiplier)
{
	OnCrosshairSpread.Broadcast(BaseSpreadAngle, SpreadAngleMultiplier);
}

void UMultiplayerShooterOverlayController::SetTarget(const AActor* NewTarget, ETeamAttitude::Type Attitude) const
{
	OnTargetChanged.Broadcast(NewTarget, Attitude);
}
