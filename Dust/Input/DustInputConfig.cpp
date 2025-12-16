#include "Input/DustInputConfig.h"
#include "DustLogChannels.h"

UDustInputConfig::UDustInputConfig(const FObjectInitializer& ObjectInitializer)
{
}

const UInputAction* UDustInputConfig::FindNativeInputActionWithTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FDustInputAction& Action : NativeInputActions)
	{
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction.Get();
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(
			LogDust,
			Error,
			TEXT("Can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."), 
			*InputTag.ToString(), 
			*GetNameSafe(this)
		);
	}
	
	return nullptr;
}

const UInputAction* UDustInputConfig::FindAbilityInputActionWithTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FDustInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction.Get();
		}
	}
	
	if (bLogNotFound)
	{
		UE_LOG(
			LogDust,
			Error,
			TEXT("Can't find AbilityInputAction for InputTag [%s] on InputConfig [%s]."),
			*InputTag.ToString(),
			*GetNameSafe(this)
		);
	}

	return nullptr;
}
