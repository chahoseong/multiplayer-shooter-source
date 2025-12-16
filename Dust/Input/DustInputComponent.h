#pragma once

#include "DustInputConfig.h"
#include "EnhancedInputComponent.h"
#include "DustInputComponent.generated.h"

class UEnhancedInputLocalPlayerSubsystem;

UCLASS(Config=Input)
class DUST_API UDustInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	UDustInputComponent(const FObjectInitializer& ObjectInitializer);
	
	template <typename UserClass, typename FuncType>
	void BindNativeAction(const UDustInputConfig* InputConfig,
		const FGameplayTag& InputTag, ETriggerEvent TriggerEvent,
		UserClass* Object, FuncType&& Func, bool bLogIfNotFound)
	{
		check(InputConfig);
		
		if (const UInputAction* InputAction = InputConfig->FindNativeInputActionWithTag(InputTag, bLogIfNotFound))
		{
			BindAction(InputAction, TriggerEvent, Object, Forward<FuncType>(Func));
		}
	}
	
	template <typename UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const UDustInputConfig* InputConfig,
		UserClass* Object, PressedFuncType&& PressedFunc, ReleasedFuncType&& ReleasedFunc,
		TArray<uint32>& BindHandles)
	{
		check(InputConfig);
		
		for (const FDustInputAction& Action : InputConfig->AbilityInputActions)
		{
			if (Action.InputAction && Action.InputTag.IsValid())
			{
				if (PressedFunc)
				{
					BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, Forward<PressedFuncType>(PressedFunc), Action.InputTag).GetHandle());
				}
				
				if (ReleasedFunc)
				{
					BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Completed, Object, Forward<ReleasedFuncType>(ReleasedFunc), Action.InputTag).GetHandle());
				}
			}
		}
	}
	
	void RemoveBinds(TArray<uint32>& BindHandles);
};
