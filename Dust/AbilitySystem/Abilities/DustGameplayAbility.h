#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "DustGameplayAbility.generated.h"

class UDustCameraMode;

UENUM(BlueprintType)
enum class EDustAbilityActivationPolicy : uint8
{
	OnInputTriggered,
	WhileInputActive,
	OnGranted,
	Manually,
};

UCLASS()
class DUST_API UDustGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UDustGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UFUNCTION(BlueprintCallable, Category="Dust|Ability")
	void SetCameraMode(TSubclassOf<UDustCameraMode> CameraMode);
	
	UFUNCTION(BlueprintCallable, Category="Dust|Ability")
	void ClearCameraMode();
	
	EDustAbilityActivationPolicy GetActivationPolicy() const;
	
	UFUNCTION(BlueprintPure, Category="Dust|Ability")
	AController* GetControllerFromActorInfo() const;
	
	UFUNCTION(BlueprintPure, Category="Dust|Ability", meta=(DeterminesOutputType=ControllerType))
	AController* GetTypedControllerFromActorInfo(TSubclassOf<AController> ControllerType) const;
	
	UFUNCTION(BlueprintPure, Category="Dust|Ability")
	ACharacter* GetCharacterFromActorInfo() const;
	
	UFUNCTION(BlueprintPure, Category="Dust|Ability", meta=(DeterminesOutputType=CharacterType))
	ACharacter* GetTypedCharacterFromActorInfo(TSubclassOf<ACharacter> CharacterType) const;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dust|Activation")
	EDustAbilityActivationPolicy ActivationPolicy;
	
private:
	TSubclassOf<UDustCameraMode> ActiveCameraMode;
};
