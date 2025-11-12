#include "Player/MultiplayerShooterPlayerState.h"
#include "AbilitySystem/MultiplayerShooterAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/MultiplayerShooterCombatSet.h"
#include "AbilitySystem/Attributes/MultiplayerShooterHealthSet.h"
#include "Net/UnrealNetwork.h"

AMultiplayerShooterPlayerState::AMultiplayerShooterPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UMultiplayerShooterAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	HealthSet = CreateDefaultSubobject<UMultiplayerShooterHealthSet>(TEXT("HealthSet"));
	CombatSet = CreateDefaultSubobject<UMultiplayerShooterCombatSet>(TEXT("CombatSet"));
	
	// AbilitySystemComponent needs to be updated at a high frequency.
	SetNetUpdateFrequency(100.0f);

	MyTeamId = FGenericTeamId::NoTeam;
}

void AMultiplayerShooterPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, MyTeamId);
}

UAbilitySystemComponent* AMultiplayerShooterPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UMultiplayerShooterHealthSet* AMultiplayerShooterPlayerState::GetHealthSet() const
{
	return HealthSet;
}

UMultiplayerShooterCombatSet* AMultiplayerShooterPlayerState::GetCombatSet() const
{
	return CombatSet;
}

void AMultiplayerShooterPlayerState::SetGenericTeamId(const FGenericTeamId& TeamId)
{
	if (HasAuthority())
	{
		const FGenericTeamId OldTeamId = MyTeamId;
		// MyTeamId가 변경됐음을 수동으로 엔진에게 알려서
		// 바로 복제가 되도록 합니다.
		// 이거를 사용하는 이유는 구조체의 경우, 멤버가 바뀌었을 때,
		// 엔진에서 변화를 감지하지 못하기 때문입니다.
		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, MyTeamId, this);
		MyTeamId = TeamId;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot set team for %s on non-authority"), *GetPathName(this));
	}
}

FGenericTeamId AMultiplayerShooterPlayerState::GetGenericTeamId() const
{
	return MyTeamId;
}

bool AMultiplayerShooterPlayerState::HasStartupAbilities() const
{
	return !GrantedHandles.IsEmpty();
}

void AMultiplayerShooterPlayerState::GiveStartupAbilities(const UMultiplayerShooterAbilitySet* AbilitySet)
{
	check(AbilitySet);
	AbilitySet->GiveToAbilitySystem(GrantedHandles, AbilitySystemComponent, this);
}

void AMultiplayerShooterPlayerState::OnRep_MyTeamId(FGenericTeamId OldTeamId)
{

}
