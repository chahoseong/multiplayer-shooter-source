#include "Camera/MultiplayerShooterCameraStateComponent.h"
#include "Camera/MultiplayerShooterCameraMode.h"
#include "Camera/MultiplayerShooterCameraModeStack.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

UMultiplayerShooterCameraStateComponent* UMultiplayerShooterCameraStateComponent::FindCameraStateComponent(
	const AActor* Actor)
{
	return IsValid(Actor) ? Actor->FindComponentByClass<UMultiplayerShooterCameraStateComponent>() : nullptr;
}

UMultiplayerShooterCameraStateComponent::UMultiplayerShooterCameraStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMultiplayerShooterCameraStateComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsOwnerLocallyControlled())
	{
		if (!CameraModeStack)
		{
			CameraModeStack = NewObject<UMultiplayerShooterCameraModeStack>(this);
			check(CameraModeStack);
		}
		SpringArmComponent = GetOwner()->FindComponentByClass<USpringArmComponent>();
		CameraComponent = GetOwner()->FindComponentByClass<UCameraComponent>();
	}
	else
	{
		SetComponentTickEnabled(false);
		Deactivate();
	}
}

bool UMultiplayerShooterCameraStateComponent::IsOwnerLocallyControlled() const
{
	const APawn* OwningPawn = GetOwner<APawn>();
	return IsValid(OwningPawn) ? OwningPawn->IsLocallyControlled() : false;
}

void UMultiplayerShooterCameraStateComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!CameraModeStack || !CameraModeStack->IsActive())
	{
		return;
	}

	UpdateCameraModes();

	FMultiplayerShooterCameraModeView CameraModeView;
	CameraModeStack->EvaluateStack(DeltaTime, CameraModeView);

	if (APawn* TargetPawn = Cast<APawn>(GetOwner()))
	{
		if (APlayerController* PlayerController = TargetPawn->GetController<APlayerController>())
		{
			PlayerController->SetControlRotation(CameraModeView.ControlRotation);
		}
	}
	
	SpringArmComponent->SocketOffset = CameraModeView.SocketOffset;
	SpringArmComponent->TargetArmLength = CameraModeView.TargetArmLength;

	CameraComponent->SetFieldOfView(CameraModeView.FieldOfView);
}

void UMultiplayerShooterCameraStateComponent::UpdateCameraModes() const
{
	if (DetermineCameraModeDelegate.IsBound())
	{
		if (TSubclassOf CameraMode = DetermineCameraModeDelegate.Execute())
		{
			CameraModeStack->PushCameraMode(CameraMode);
		}
	}
}
