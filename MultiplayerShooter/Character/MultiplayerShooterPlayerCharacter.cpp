#include "Character/MultiplayerShooterPlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "MultiplayerShooterGameplayTags.h"
#include "Animation/MultiplayerShooterAnimInstance.h"
#include "Player/MultiplayerShooterPlayerState.h"

void AMultiplayerShooterPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitializeAbilitySystem();

	if (AimingAbility)
	{
		FGameplayAbilitySpec AbilitySpec(AimingAbility);
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(MultiplayerShooterGameplayTags::Input_Action_Aim);
		AbilitySystemComponent->GiveAbility(AbilitySpec);
	}
}

void AMultiplayerShooterPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitializeAbilitySystem();
}

void AMultiplayerShooterPlayerCharacter::InitializeAbilitySystem()
{
	AMultiplayerShooterPlayerState* MultiplayerShooterPlayerState =
	GetPlayerStateChecked<AMultiplayerShooterPlayerState>();
	AbilitySystemComponent = MultiplayerShooterPlayerState->GetAbilitySystemComponent();
	AbilitySystemComponent->InitAbilityActorInfo(MultiplayerShooterPlayerState, this);

	UMultiplayerShooterAnimInstance* MultiplayerShooterAnimInstance =
		Cast<UMultiplayerShooterAnimInstance>(GetMesh()->GetAnimInstance());
	if (MultiplayerShooterAnimInstance)
	{
		MultiplayerShooterAnimInstance->InitializeWithAbilitySystem(AbilitySystemComponent);
	}
}
