#include "Animation/MultiplayerShooterAnimInstance.h"
#include "AbilitySystemGlobals.h"
#include "Character/MultiplayerShooterCharacter.h"
#include "Character/MultiplayerShooterCharacterMovementComponent.h"
#include "Misc/DataValidation.h"

UMultiplayerShooterAnimInstance::UMultiplayerShooterAnimInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMultiplayerShooterAnimInstance::InitializeWithAbilitySystem(UAbilitySystemComponent* AbilitySystem)
{
	check(AbilitySystem);
	GameplayTagPropertyMap.Initialize(this, AbilitySystem);
}

void UMultiplayerShooterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (const AActor* OwningActor = GetOwningActor())
	{
		if (UAbilitySystemComponent* AbilitySystem = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor))
		{
			InitializeWithAbilitySystem(AbilitySystem);
		}
	}
}

void UMultiplayerShooterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (const AMultiplayerShooterCharacter* Character = Cast<AMultiplayerShooterCharacter>(GetOwningActor()))
	{
		const UMultiplayerShooterCharacterMovementComponent* CharacterMovement =
			CastChecked<UMultiplayerShooterCharacterMovementComponent>(Character->GetCharacterMovement());
		const FMultiplayerShooterCharacterGroundInfo& GroundInfo = CharacterMovement->GetGroundInfo();
		GroundDistance = GroundInfo.GroundDistance;
	}
}

#if WITH_EDITOR
EDataValidationResult UMultiplayerShooterAnimInstance::IsDataValid(FDataValidationContext& Context) const
{
	Super::IsDataValid(Context);
	GameplayTagPropertyMap.IsDataValid(this, Context);
	return Context.GetNumErrors() > 0 ? EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
#endif