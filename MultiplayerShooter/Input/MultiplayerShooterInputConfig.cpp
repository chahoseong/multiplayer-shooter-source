#include "Input/MultiplayerShooterInputConfig.h"
#include "InputAction.h"

UMultiplayerShooterInputConfig::UMultiplayerShooterInputConfig(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

const UInputAction* UMultiplayerShooterInputConfig::FindNativeInputActionWithTag(const FGameplayTag& InputTag,
	bool bLogNotFound) const
{
	for (const FMultiplayerShooterInputAction& Action : NativeInputActions)
	{
		if (IsValid(Action.InputAction) && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT("Can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."),
			*InputTag.ToString(), *GetNameSafe(this)
		);
	}

	return nullptr;
}

const UInputAction* UMultiplayerShooterInputConfig::FindAbilityInputActionWithTag(const FGameplayTag& InputTag,
	bool bLogNotFound) const
{
	for (const FMultiplayerShooterInputAction& Action : AbilityInputActions)
	{
		if (IsValid(Action.InputAction) && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT("Can't find AbilityInputAction for InputTag [%s] on InputConfig [%s]."),
			*InputTag.ToString(), *GetNameSafe(this)
		);
	}

	return nullptr;
}
