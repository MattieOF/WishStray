// copyright lololol
// ReSharper disable CppMemberFunctionMayBeConst

#include "Core/Character/BengalController.h"

#include "WishStray.h"
#include "Core/Character/BengalCharacter.h"

ABengalController::ABengalController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABengalController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bChargingJump)
	{
		if (JumpCharge < 1)
			JumpCharge = FMath::Min(JumpCharge + DeltaSeconds, 1);
		
		// Recalculate jump
		ABengalCharacter* Bengal = Cast<ABengalCharacter>(GetPawn());
		if (!Bengal)
		{
			UE_LOG(LogBengal, Error, TEXT("What the fuck!! bengal is null"));
			return;
		}

		FVector Forward = Bengal->CameraBoom->GetForwardVector();
		FVector EndLoc = Bengal->GetActorLocation() + Forward * FMath::InterpEaseOut(100, 1500, JumpCharge, 4);
		FHitResult Hit;
		FCollisionQueryParams QueryParams = FCollisionQueryParams::DefaultQueryParam;
		QueryParams.AddIgnoredActor(Bengal);
		if (GetWorld()->LineTraceSingleByChannel(Hit, Bengal->GetActorLocation(), EndLoc, ECC_WorldStatic, QueryParams))
		{
			
		}
		DrawDebugLine(GetWorld(), Bengal->GetActorLocation(), EndLoc, FColor::Red, false, DeltaSeconds, 0, 5);
	}
	else
		JumpCharge = 0;
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
	Cast<ABengalCharacter>(GetPawn())->AddCameraInput(Value, 0);
	// GetPawn()->AddControllerPitchInput(Value);
}

void ABengalController::OnHorizontalLook(float Value)
{
	Cast<ABengalCharacter>(GetPawn())->AddCameraInput(0, Value);
	// GetPawn()->AddControllerYawInput(Value);
}

void ABengalController::OnBeginJump()
{
	bChargingJump = true;
}

void ABengalController::OnEndJump()
{
	bChargingJump = false;
}
