#include "Animation/DustAnimInstance.h"
#include "AbilitySystemGlobals.h"
#include "Expected.h"
#include "GameFramework/Character.h"
#include "Misc/DataValidation.h"

UDustAnimInstance::UDustAnimInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UDustAnimInstance::InitializeWithAbilitySystem(UAbilitySystemComponent* AbilitySystem)
{
	check(AbilitySystem);
	
	GameplayTagPropertyMap.Initialize(this, AbilitySystem);
}

void UDustAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	Expect(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwningActor()))
	.AndThen([this](UAbilitySystemComponent* AbilitySystemComponent){
		InitializeWithAbilitySystem(AbilitySystemComponent);
	});
}

void UDustAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	// Expect(Cast<ACharacter>(GetOwningActor()), IsValid, TEXT(""))
	// .AndThen([](const ACharacter* Character)
	// {
	// 	
	// });
}

#if WITH_EDITOR
EDataValidationResult UDustAnimInstance::IsDataValid(FDataValidationContext& Context) const
{
	Super::IsDataValid(Context);
	
	GameplayTagPropertyMap.IsDataValid(this, Context);
	
	return Context.GetNumErrors() > 0 ? EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
#endif


