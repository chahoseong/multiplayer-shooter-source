#include "Animation/DustAnimInstance.h"
#include "AbilitySystemGlobals.h"
#include "Misc/DataValidation.h"

UDustAnimInstance::UDustAnimInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UDustAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UDustAnimInstance::InitializeWithAbilitySystem(UAbilitySystemComponent* AbilitySystem)
{
	check(AbilitySystem);
	
	GameplayTagPropertyMap.Initialize(this, AbilitySystem);
}

void UDustAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

#if WITH_EDITOR
EDataValidationResult UDustAnimInstance::IsDataValid(FDataValidationContext& Context) const
{
	Super::IsDataValid(Context);
	
	GameplayTagPropertyMap.IsDataValid(this, Context);
	
	return Context.GetNumErrors() > 0 ? EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
#endif

void UDustAnimInstance::SetLeftHandEffectorLocation(const FVector& WorldLocation)
{
	FVector BoneLocation;
	FRotator BoneRotation;
	GetSkelMeshComponent()->TransformToBoneSpace(
		LeftHandIK, 
		WorldLocation, FRotator::ZeroRotator,
		BoneLocation, BoneRotation
	);
	
	LeftHandEffectorLocation = BoneLocation;
}
