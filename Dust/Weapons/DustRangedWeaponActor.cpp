#include "Weapons/DustRangedWeaponActor.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "DustGameplayTags.h"
#include "Animation/DustAnimInstance.h"
#include "GameFramework/Character.h"

ADustRangedWeaponActor::ADustRangedWeaponActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(MeshComponent);
}

void ADustRangedWeaponActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (UDustAnimInstance* OwnerAnimInstance = GetOwnerAnimInstance())
	{
		const FTransform GripTransform = MeshComponent->GetSocketTransform(GripEffector.SocketName);
		OwnerAnimInstance->SetLeftHandEffectorLocation(GripTransform.TransformPosition(GripEffector.TargetLocation));
	}
}

UDustAnimInstance* ADustRangedWeaponActor::GetOwnerAnimInstance() const
{
	if (const ACharacter* OwningCharacter = GetOwner<ACharacter>())
	{
		const USkeletalMeshComponent* Mesh = OwningCharacter->GetMesh();
		return Mesh ? Cast<UDustAnimInstance>(Mesh->GetAnimInstance()) : nullptr;
	}
		
	return nullptr;
}

bool ADustRangedWeaponActor::IsAiming() const
{
	UAbilitySystemComponent* AbilitySystem = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner());
	return AbilitySystem ? AbilitySystem->HasMatchingGameplayTag(DustGameplayTags::Status_Aiming) : false;
}