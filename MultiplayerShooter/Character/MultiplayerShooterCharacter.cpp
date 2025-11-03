#include "Character/MultiplayerShooterCharacter.h"
#include "AbilitySystemComponent.h"
#include "MultiplayerShooterCharacterMovementComponent.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystem/MultiplayerShooterAbilitySet.h"
#include "Components/CapsuleComponent.h"
#include "Physics/MultiplayerShooterCollisionChannels.h"
#include "Player/MultiplayerShooterPlayerState.h"

AMultiplayerShooterCharacter::AMultiplayerShooterCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UMultiplayerShooterCharacterMovementComponent>(CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Collision
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(MultiplayerShooter_ObjectChannel_Projectile, ECR_Ignore);

	GetMesh()->SetCollisionObjectType(MultiplayerShooter_ObjectChannel_Character);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(MultiplayerShooter_ObjectChannel_Projectile, ECR_Block);

	// Movement
	UMultiplayerShooterCharacterMovementComponent* MultiplayerShooterCharacterMovement =
		CastChecked<UMultiplayerShooterCharacterMovementComponent>(GetCharacterMovement());
	MultiplayerShooterCharacterMovement->GravityScale = 1.0f;
	MultiplayerShooterCharacterMovement->MaxAcceleration = 2400.0f;
	MultiplayerShooterCharacterMovement->BrakingFrictionFactor = 1.0f;
	MultiplayerShooterCharacterMovement->BrakingFriction = 6.0f;
	MultiplayerShooterCharacterMovement->GroundFriction = 8.0f;
	MultiplayerShooterCharacterMovement->BrakingDecelerationWalking = 1400.0f;
	MultiplayerShooterCharacterMovement->bUseControllerDesiredRotation = false;
	MultiplayerShooterCharacterMovement->bOrientRotationToMovement = false;
	MultiplayerShooterCharacterMovement->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	MultiplayerShooterCharacterMovement->bAllowPhysicsRotationDuringAnimRootMotion = false;
	MultiplayerShooterCharacterMovement->GetNavAgentPropertiesRef().bCanCrouch = true;
	MultiplayerShooterCharacterMovement->bCanWalkOffLedgesWhenCrouching = true;
	MultiplayerShooterCharacterMovement->SetCrouchedHalfHeight(65.0f);
}

void AMultiplayerShooterCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitializeAbilitySystem();

	if (IsValid(InitialAbilitySet))
	{
		FMultiplayerShooterAbilitySet_GrantedHandles GrantedHandles;
		InitialAbilitySet->GiveToAbilitySystem(GrantedHandles, AbilitySystemComponent);
	}
}

void AMultiplayerShooterCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	InitializeAbilitySystem();
}

void AMultiplayerShooterCharacter::InitializeAbilitySystem()
{
}

void AMultiplayerShooterCharacter::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> EffectToApply, float Level) const
{
	const FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	const FGameplayEffectSpecHandle EffectSpec = AbilitySystemComponent->MakeOutgoingSpec(EffectToApply, Level, EffectContext);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data);
}

UAbilitySystemComponent* AMultiplayerShooterCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UMultiplayerShooterHealthSet* AMultiplayerShooterCharacter::GetHealthSet() const
{
	return HealthSet;
}

void AMultiplayerShooterCharacter::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	if (GetController())
	{
		if (HasAuthority())
		{
			AMultiplayerShooterPlayerState* MultiplayerShooterPlayerState =
				GetPlayerState<AMultiplayerShooterPlayerState>();
			MultiplayerShooterPlayerState->SetGenericTeamId(TeamID);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("You can't set the team ID on a character (%s) except on the authority"), *GetPathNameSafe(this));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("You can't set the team ID on a possessed character (%s); it's driven by the associated controller"), *GetPathNameSafe(this));
	}
}

FGenericTeamId AMultiplayerShooterCharacter::GetGenericTeamId() const
{
	const AMultiplayerShooterPlayerState* MultiplayerShooterPlayerState =
		GetPlayerState<AMultiplayerShooterPlayerState>();
	if (MultiplayerShooterPlayerState)
	{
		return MultiplayerShooterPlayerState->GetGenericTeamId();
	}
	return FGenericTeamId::NoTeam;
}
