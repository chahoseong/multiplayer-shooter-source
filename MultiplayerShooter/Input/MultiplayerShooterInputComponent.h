#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "GameplayTagContainer.h"
#include "MultiplayerShooterInputConfig.h"
#include "MultiplayerShooterInputComponent.generated.h"

class UMultiplayerShooterInputConfig;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MULTIPLAYERSHOOTER_API UMultiplayerShooterInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template <typename UserClass, typename FuncType>
	void BindNativeAction(
		const UMultiplayerShooterInputConfig* InputConfig,
		const FGameplayTag& InputTag,
		ETriggerEvent TriggerEvent,
		UserClass* Object,
		FuncType Func,
		bool bLogIfNotFound
	);

	template <typename UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(
		const UMultiplayerShooterInputConfig* InputConfig,
		UserClass* Object,
		PressedFuncType PressedFunc,
		ReleasedFuncType ReleasedFunc
	);
};

template <typename UserClass, typename FuncType>
void UMultiplayerShooterInputComponent::BindNativeAction(const UMultiplayerShooterInputConfig* InputConfig,
	const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound)
{
	check(InputConfig);

	if (const UInputAction* InputAction = InputConfig->FindNativeInputActionWithTag(InputTag, bLogIfNotFound))
	{
		BindAction(InputAction, TriggerEvent, Object, Func);
	}
}

template <typename UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UMultiplayerShooterInputComponent::BindAbilityActions(const UMultiplayerShooterInputConfig* InputConfig,
	UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc)
{
	check(InputConfig);

	for (const FMultiplayerShooterInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (IsValid(Action.InputAction) && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, PressedFunc, Action.InputTag);
			}

			if (ReleasedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			}
		}
	}
}
