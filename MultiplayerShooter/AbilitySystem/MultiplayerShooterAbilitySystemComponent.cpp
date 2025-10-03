#include "AbilitySystem/MultiplayerShooterAbilitySystemComponent.h"

#include "MultiplayerShooterGameplayTags.h"
#include "Abilities/MultiplayerShooterGameplayAbility.h"

UMultiplayerShooterAbilitySystemComponent::UMultiplayerShooterAbilitySystemComponent(
	const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ClearAbilityInput();
}

void UMultiplayerShooterAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
	if (HasMatchingGameplayTag(MultiplayerShooterGameplayTags::Gameplay_AbilityInputBlocked))
	{
		ClearAbilityInput();
		return;
	}
	
	static TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;
	AbilitiesToActivate.Reset();
	
	// Process all abilities that activate when the input is held.
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputHeldSpecHandles)
	{
		if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (!IsValid(AbilitySpec->Ability) || AbilitySpec->IsActive())
			{
				continue;
			}

			if (IsAbilityActivationPolicy(AbilitySpec->Ability, EMultiplayerShooterAbilityActivationPolicy::WhileInputActive))
			{
				AbilitiesToActivate.Add(AbilitySpec->Handle);
			}
		}
	}

	// Process all abilities that had their input pressed this frame.
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (!IsValid(AbilitySpec->Ability))
			{
				continue;
			}

			AbilitySpec->InputPressed = true;

			if (AbilitySpec->IsActive())
			{
				AbilitySpecInputPressed(*AbilitySpec);
			}
			else if (IsAbilityActivationPolicy(AbilitySpec->Ability, EMultiplayerShooterAbilityActivationPolicy::OnInputTriggered))
			{
				AbilitiesToActivate.Add(AbilitySpec->Handle);
			}
		}
	}

	// Try to activate all the abilities that are from presses and holds.
	// We do it all at once so that held inputs don't activate the ability
	// and then also send input event to the ability because of the press.
	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : AbilitiesToActivate)
	{
		TryActivateAbility(AbilitySpecHandle);
	}

	// Process all abilities that had their input released this frame.
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (!IsValid(AbilitySpec->Ability))
			{
				continue;
			}

			AbilitySpec->InputPressed = false;

			if (AbilitySpec->IsActive())
			{
				AbilitySpecInputReleased(*AbilitySpec);
			}
		}
	}

	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}

bool UMultiplayerShooterAbilitySystemComponent::IsAbilityActivationPolicy(const UGameplayAbility* AbilityToCheck,
                                                                          EMultiplayerShooterAbilityActivationPolicy ActivationPolicy) const
{
	const UMultiplayerShooterGameplayAbility* MultiplayerShooterGameplayAbility =
		Cast<UMultiplayerShooterGameplayAbility>(AbilityToCheck);
	return IsValid(MultiplayerShooterGameplayAbility)
		       ? MultiplayerShooterGameplayAbility->GetActivationPolicy() == ActivationPolicy
		       : false;
}

void UMultiplayerShooterAbilitySystemComponent::ClearAbilityInput()
{
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
}

void UMultiplayerShooterAbilitySystemComponent::AbilityInputPressed(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (IsValid(AbilitySpec.Ability) && AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
			{
				InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
			}
		}
	}
}

void UMultiplayerShooterAbilitySystemComponent::AbilityInputReleased(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (IsValid(AbilitySpec.Ability) && AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
			{
				InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.Remove(AbilitySpec.Handle);
			}
		}
	}
}

void UMultiplayerShooterAbilitySystemComponent::AbilitySpecInputPressed(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputPressed(Spec);

	// We don't support UGameplayAbility::bReplicateInputDirectly.
	// Use replicated events instead so that the WaitInputPress ability task works.
	if (Spec.IsActive())
	{
		if (const UGameplayAbility* Instance = Spec.GetPrimaryInstance())
		{
			FPredictionKey OriginalPredictionKey = Instance->GetCurrentActivationInfo().GetActivationPredictionKey();
			// Invoke the InputPressed event. This is not replicated here. If someone is listening, they may replicate the InputPressed event to the server.
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, OriginalPredictionKey);
		}
	}
}

void UMultiplayerShooterAbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputReleased(Spec);

	// We don't support UGameplayAbility::bReplicateInputDirectly.
	// Use replicated events instead so that the WaitInputRelease ability task works.
	if (Spec.IsActive())
	{
		if (const UGameplayAbility* Instance = Spec.GetPrimaryInstance())
		{
			FPredictionKey OriginalPredictionKey = Instance->GetCurrentActivationInfo().GetActivationPredictionKey();
			// Invoke the InputReleased event. This is not replicated here. If someone is listening, they may replicate the InputReleased event to the server.
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, OriginalPredictionKey);
		}
	}
}
