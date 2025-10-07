#include "Character/MultiplayerShooterPlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "MultiplayerShooterGameplayTags.h"
#include "Animation/MultiplayerShooterAnimInstance.h"
#include "Equipment/MultiplayerShooterEquipmentManagerComponent.h"
#include "Player/MultiplayerShooterPlayerState.h"

AMultiplayerShooterPlayerCharacter::AMultiplayerShooterPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	EquipmentManagerComponent = CreateDefaultSubobject<UMultiplayerShooterEquipmentManagerComponent>(TEXT("EquipmentManagerComponent"));
}

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

	for (TSubclassOf EquipmentDefinition : StartupEquipments)
	{
		EquipmentManagerComponent->Equip(EquipmentDefinition);
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
