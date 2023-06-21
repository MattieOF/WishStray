// copyright lololol
// ReSharper disable CppMemberFunctionMayBeConst

#include "Core/Character/BengalController.h"

#include "WishStray.h"
#include "Components/InputComponent.h"
#include "Components/SplineComponent.h"
#include "Core/BengalStatics.h"
#include "Core/Character/BengalCharacter.h"
#include "Visual/PawPrint.h"

ABengalController::ABengalController()
{
	PrimaryActorTick.bCanEverTick = true;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Jumping Spline"));
}

void ABengalController::BeginPlay()
{
	Super::BeginPlay();

	PawPrint = GetWorld()->SpawnActor<APawPrint>();
}

void ABengalController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bJumping && bChargingJump)
	{
		// if (JumpCharge < 1)
		// 	JumpCharge = FMath::Min(JumpCharge + DeltaSeconds, 1);
		
		// Recalculate jump
		const ABengalCharacter* Bengal = Cast<ABengalCharacter>(GetPawn());
		if (!Bengal)
		{
			UE_LOG(LogBengal, Error, TEXT("What the fuck!! bengal is null"));
			return;
		}

		// FVector Forward = Bengal->CameraBoom->GetForwardVector();
		// FVector StartLoc = Bengal->Camera->GetComponentLocation();
		// FVector EndLoc = StartLoc + Forward * (FMath::InterpEaseOut(100, 1500, JumpCharge, 4) + Bengal->CameraBoom->TargetArmLength);
		// FHitResult Hit;
		// FCollisionQueryParams QueryParams = FCollisionQueryParams::DefaultQueryParam;
		// QueryParams.AddIgnoredActor(Bengal);
		// DrawDebugLine(GetWorld(), StartLoc, EndLoc, FColor::Yellow);
		// GetWorld()->LineTraceSingleByChannel(Hit, StartLoc, EndLoc, ECC_WorldStatic, QueryParams);
		// FVector HighPoint = Hit.bBlockingHit ? Hit.ImpactPoint : EndLoc;
		// DrawDebugLine(GetWorld(), HighPoint, HighPoint + FVector(0, 0, -10000), FColor::Blue);
		// if (GetWorld()->LineTraceSingleByChannel(Hit, HighPoint, HighPoint + FVector(0, 0, -100000), ECC_WorldStatic, QueryParams))
		// {
		// 	DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 30, 6, FColor::Red);
		// } else
		// {
		// 	UE_LOG(LogBengal, Error, TEXT("Bad things are happening: jump target location trace failed."));
		// }

		FVector JumpToLocation;
		if (GetJumpToLocation(JumpToLocation))
		{
			PawPrint->SetActorLocation(JumpToLocation + FVector(0, 0, 6), false, nullptr, ETeleportType::TeleportPhysics);
			PawPrint->SetActorRotation(FRotator(0, (JumpToLocation - Bengal->GetActorLocation()).GetSafeNormal().Rotation().Yaw + 90, 0));
			Bengal->GetMesh()->SetRelativeRotation(FRotator(0, (JumpToLocation - Bengal->GetMesh()->GetComponentLocation()).GetSafeNormal().Rotation().Yaw - 90, 0));
		}
	}
	else
		JumpCharge = 0;

	if (bJumping)
	{
		if (JumpProgress >= Spline->Duration)
		{
			bJumping = false;
			const ABengalCharacter* Bengal = Cast<ABengalCharacter>(GetPawn());
			FRotator Rot = Bengal->GetMesh()->GetComponentRotation();
			Rot.Roll = 0;
			Rot.Pitch = 0;
			Bengal->GetMesh()->SetWorldRotation(Rot);
			Bengal->GetMesh()->SetPhysicsLinearVelocity(FVector::ZeroVector);
			GetPawn()->SetActorEnableCollision(true);
		} else
		{
			GetPawn()->SetActorLocation(Spline->GetWorldLocationAtTime(JumpProgress));
			FRotator Rot = Spline->GetWorldDirectionAtTime(JumpProgress).Rotation();
			Rot.Yaw -= 90;
			Cast<ABengalCharacter>(GetPawn())->GetMesh()->SetWorldRotation(Rot);
		
			JumpProgress += DeltaSeconds;
		}
	}
}

void ABengalController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("VerticalMove", this, &ABengalController::OnVerticalMovement);
	InputComponent->BindAxis("HorizontalMove", this, &ABengalController::OnHorizontalMovement);
	InputComponent->BindAxis("VerticalLook", this, &ABengalController::OnVerticalLook);
	InputComponent->BindAxis("HorizontalLook", this, &ABengalController::OnHorizontalLook);
	InputComponent->BindAxis("Scroll", this, &ABengalController::OnScroll);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ABengalController::OnBeginJump);
	InputComponent->BindAction("Jump", IE_Released, this, &ABengalController::OnEndJump);
}

void ABengalController::OnVerticalMovement(float Value)
{
	if (bChargingJump)
	{
		FVector Forward = Cast<ABengalCharacter>(GetPawn())->CameraBoom->GetForwardVector();
		Forward.Z = 0;
		JumpPosOffset += (Forward.GetSafeNormal() * GetWorld()->DeltaRealTimeSeconds * Value * 1000);
		JumpPosOffset = JumpPosOffset.GetClampedToMaxSize(700);
	}
	else if (!bJumping)
		GetPawn()->AddMovementInput(FVector::ForwardVector, Value);
}

void ABengalController::OnHorizontalMovement(float Value)
{
	if (bChargingJump)
	{
		FVector Right = Cast<ABengalCharacter>(GetPawn())->CameraBoom->GetRightVector();
		Right.Z = 0;
		JumpPosOffset += (Right.GetSafeNormal() * static_cast<double>(GetWorld()->DeltaRealTimeSeconds) * Value * 1000);
		JumpPosOffset = JumpPosOffset.GetClampedToMaxSize(700);
	}
	else if (!bJumping)
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

void ABengalController::OnScroll(float Value)
{
	USpringArmComponent* Boom = Cast<ABengalCharacter>(GetPawn())->CameraBoom;
	Boom->TargetArmLength = FMath::Clamp(Boom->TargetArmLength + Value * 100, 300.0f, 1500.0f);
}

void ABengalController::OnBeginJump()
{
	if (bJumping || bChargingJump)
		return;
	
	JumpPosOffset = FVector::ZeroVector;
	bChargingJump = true;
	PawPrint->SetVisible(true);
}

void ABengalController::OnEndJump()
{
	if (!bChargingJump)
		return;
	
	bChargingJump = false;
	PawPrint->SetVisible(false);

	if (JumpPosOffset.Length() < 75)
		return;
	
	FVector JumpToLocation;
	bHasJumpLocation = GetJumpToLocation(JumpToLocation);
	if (bHasJumpLocation)
	{
		const FVector StartPos = GetPawn()->GetActorLocation();
		const FVector EndPos = JumpToLocation + FVector(0, 0, UBengalStatics::GetZUnderOrigin(GetPawn()) + 5);
		
		// Generate path
		Spline->ClearSplinePoints();
		Spline->AddSplinePoint(StartPos, ESplineCoordinateSpace::World);
		Spline->AddSplinePoint(EndPos, ESplineCoordinateSpace::World);

		FVector Dir = (EndPos - StartPos).GetSafeNormal() * 150;
		FVector ReverseDir = -Dir;
		Spline->SetTangentAtSplinePoint(0, Dir, ESplineCoordinateSpace::World);
		Spline->SetTangentAtSplinePoint(1, ReverseDir, ESplineCoordinateSpace::World);
		int Tries = 6;

		TArray<AActor*> Ignored;
		Ignored.Add(GetPawn());
		do
		{
			Tries--;

			Dir = FVector(Dir.X * 1.5, Dir.Y * 1.5, (Dir.Z + 50) * 3);
			Spline->SetTangentAtSplinePoint(0, Dir, ESplineCoordinateSpace::World);
			ReverseDir = Spline->GetTangentAtSplinePoint(0, ESplineCoordinateSpace::World);
			ReverseDir.Z = -ReverseDir.Z;
			Spline->SetTangentAtSplinePoint(1, ReverseDir, ESplineCoordinateSpace::World);
		} while (Tries > 0 && UBengalStatics::CheckForCollisionsAlongSpline(Spline, ECC_WorldStatic, Ignored));

		if (!UBengalStatics::CheckForCollisionsAlongSpline(Spline, ECC_WorldStatic, Ignored))
		{
			// We found a path, let's use it
			JumpProgress = 0;
			Spline->Duration = Spline->GetSplineLength() / 1000;
			bJumping = true;
			GetPawn()->SetActorEnableCollision(false);
		}
	}
}

bool ABengalController::GetJumpToLocation(FVector& OutPos)
{
	ABengalCharacter* Bengal = Cast<ABengalCharacter>(GetPawn());
	
	FHitResult Hit;
	FCollisionQueryParams QueryParams = FCollisionQueryParams::DefaultQueryParam;
	QueryParams.AddIgnoredActor(Bengal);
	bool DidHit = GetWorld()->LineTraceSingleByChannel(Hit, Bengal->GetActorLocation() + JumpPosOffset + FVector(0, 0, 1000),
										 Bengal->GetActorLocation() + JumpPosOffset + FVector(0, 0, -10000),
										 ECC_WALKABLE, QueryParams);
	if (!DidHit)
		return false;
	else if (Hit.ImpactPoint.Z > Bengal->GetActorLocation().Z + FMath::GetMappedRangeValueClamped(FVector2D(0, 700), FVector2D(800, 400), JumpPosOffset.Length()))
		return false;
	else
	{
		OutPos = Hit.ImpactPoint;
		return true;
	}
}
