#include "Character/MultiplayerShooterCharacter.h"

AMultiplayerShooterCharacter::AMultiplayerShooterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMultiplayerShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMultiplayerShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMultiplayerShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

