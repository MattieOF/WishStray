// copyright lololol

#include "Core/Character/BengalCharacter.h"

#include "GameFramework/PawnMovementComponent.h"

ABengalCharacter::ABengalCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	Camera     = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraBoom->SetupAttachment(reinterpret_cast<USceneComponent*>(GetCapsuleComponent()));
	CameraBoom->bUsePawnControlRotation = true;
	Camera->SetupAttachment(CameraBoom);
}

void ABengalCharacter::AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce)
{
	FVector FinalDir = WorldDirection * ScaleValue * Speed;
	if (UPawnMovementComponent* MovementComponent = GetMovementComponent())
	{
		MovementComponent->AddInputVector(FinalDir, bForce);
	}
	else
	{
		Internal_AddMovementInput(FinalDir, bForce);
	}
}

void ABengalCharacter::AddCameraInput(float Vertical, float Horizontal)
{
	FRotator Rot = CameraBoom->GetRelativeRotation();
	Rot.Pitch = FMath::ClampAngle(Rot.Pitch + Vertical * GetWorld()->DeltaRealTimeSeconds * 100, -179, 179);
	Rot.Yaw += Horizontal * GetWorld()->DeltaRealTimeSeconds * 100;
	CameraBoom->SetRelativeRotation(Rot);
}
