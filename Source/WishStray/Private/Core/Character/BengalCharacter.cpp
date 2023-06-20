// copyright lololol

#include "Core/Character/BengalCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

ABengalCharacter::ABengalCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	Camera     = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraBoom->SetupAttachment(reinterpret_cast<USceneComponent*>(GetCapsuleComponent()));
	CameraBoom->bUsePawnControlRotation = true;
	Camera->SetupAttachment(CameraBoom);
}

void ABengalCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetCharacterMovement()->GetCurrentAcceleration().SizeSquared() > KINDA_SMALL_NUMBER)
	{
		GetMesh()->SetWorldRotation(UKismetMathLibrary::RInterpTo_Constant(
			GetMesh()->GetComponentRotation(),
			FRotator(0, GetCharacterMovement()->GetCurrentAcceleration().GetSafeNormal().Rotation().Yaw - 90, 0),
			DeltaSeconds, 360));
	}
}

void ABengalCharacter::AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce)
{
	FRotator Forward = Camera->GetComponentRotation();
	Forward.Pitch = 0;
	Forward.Roll = 0;

	const FVector FinalDir = Forward.RotateVector(WorldDirection) * ScaleValue * Speed;
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
	Rot.Pitch = FMath::Clamp(Rot.Pitch + Vertical * GetWorld()->DeltaRealTimeSeconds * 100, -89, 89);
	Rot.Yaw += Horizontal * GetWorld()->DeltaRealTimeSeconds * 100;
	Rot.Roll = 0;
	CameraBoom->SetRelativeRotation(Rot);
}
