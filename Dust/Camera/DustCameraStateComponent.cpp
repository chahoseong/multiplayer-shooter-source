#include "Camera/DustCameraStateComponent.h"
#include "Camera/DustCameraMode.h"
#include "Camera/DustCameraModeStack.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

UDustCameraStateComponent::UDustCameraStateComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.TickGroup = TG_PostPhysics;
}

void UDustCameraStateComponent::BeginPlay()
{
	Super::BeginPlay();
	
	SpringArmComponent = GetOwner()->FindComponentByClass<USpringArmComponent>();
	check(SpringArmComponent);
	SpringArmComponent->AddTickPrerequisiteComponent(this);
	
	CameraComponent = GetOwner()->FindComponentByClass<UCameraComponent>();
	check(CameraComponent);
}

void UDustCameraStateComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (CameraModeStack && CameraModeStack->IsActive())
	{
		UpdateCameraModes();
		
		const FDustCameraModeView CameraModeView = CameraModeStack->EvaluateStack(DeltaTime);
		
		if (const APawn* TargetPawn = Cast<APawn>(GetOwner()))
		{
			if (APlayerController* PlayerController = TargetPawn->GetController<APlayerController>())
			{
				PlayerController->SetControlRotation(CameraModeView.ControlRotation);
			}
		}
		
		SpringArmComponent->SetRelativeLocation(CameraModeView.SpringArmLocation);
		SpringArmComponent->TargetArmLength = CameraModeView.TargetArmLength;
		SpringArmComponent->SocketOffset = CameraModeView.SocketOffset;
		SpringArmComponent->TargetOffset = CameraModeView.TargetOffset;
		
		CameraComponent->SetFieldOfView(CameraModeView.FieldOfView);
	}
}

void UDustCameraStateComponent::UpdateCameraModes() const
{
	if (DetermineCameraModeDelegate.IsBound())
	{
		if (TSubclassOf CameraMode = DetermineCameraModeDelegate.Execute())
		{
			CameraModeStack->PushCameraMode(CameraMode);
		}
	}
}

void UDustCameraStateComponent::Activate(bool bReset)
{
	Super::Activate(bReset);
	
	if (!CameraModeStack)
	{
		CameraModeStack = NewObject<UDustCameraModeStack>(this);
		check(CameraModeStack);
	}
}

void UDustCameraStateComponent::Deactivate()
{
	SetComponentTickEnabled(false);
	Super::Deactivate();
}

AActor* UDustCameraStateComponent::GetTargetActor() const
{
	return GetOwner();
}
