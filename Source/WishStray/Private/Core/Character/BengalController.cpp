// copyright lololol
// ReSharper disable CppMemberFunctionMayBeConst

#include "Core/Character/BengalController.h"

#include "Core/Character/BengalCharacter.h"

ABengalController::ABengalController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABengalController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("VerticalMove", this, &ABengalController::OnVerticalMovement);
	InputComponent->BindAxis("HorizontalMove", this, &ABengalController::OnHorizontalMovement);
	InputComponent->BindAxis("VerticalLook", this, &ABengalController::OnVerticalLook);
	InputComponent->BindAxis("HorizontalLook", this, &ABengalController::OnHorizontalLook);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ABengalController::OnBeginJump);
	InputComponent->BindAction("Jump", IE_Released, this, &ABengalController::OnEndJump);
}

void ABengalController::OnVerticalMovement(float Value)
{
	GetPawn()->AddMovementInput(FVector::ForwardVector, Value);
}

void ABengalController::OnHorizontalMovement(float Value)
{
	GetPawn()->AddMovementInput(FVector::RightVector, Value);
}

void ABengalController::OnVerticalLook(float Value)
{
	// Cast<ABengalCharacter>(GetPawn())->AddCameraInput(Value, 0);
	GetPawn()->AddControllerPitchInput(Value);
}

void ABengalController::OnHorizontalLook(float Value)
{
	// Cast<ABengalCharacter>(GetPawn())->AddCameraInput(0, Value);
	GetPawn()->AddControllerYawInput(Value);
}

void ABengalController::OnBeginJump()
{
	bChargingJump = true;
}

void ABengalController::OnEndJump()
{
	bChargingJump = false;
}
