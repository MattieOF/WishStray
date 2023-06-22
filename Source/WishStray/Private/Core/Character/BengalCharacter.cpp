// copyright lololol

#include "Core/Character/BengalCharacter.h"

#include "Core/BengalGameInstance.h"
#include "Core/BreakableObject.h"
#include "Core/Character/BengalController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Visual/OutlineComponent.h"

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

	// Tick punt timer
	if (PuntTimer > 0)
		PuntTimer -= DeltaSeconds;

	// Orient to movement
	if (GetCharacterMovement()->GetCurrentAcceleration().SizeSquared() > KINDA_SMALL_NUMBER && PuntTimer < 0.5)
		TargetYaw = GetCharacterMovement()->GetCurrentAcceleration().GetSafeNormal().Rotation().Yaw - 90;
	
	GetMesh()->SetWorldRotation(UKismetMathLibrary::RInterpTo_Constant(
			GetMesh()->GetComponentRotation(),
			FRotator(0, TargetYaw, 0),
			DeltaSeconds, 360));

	// Check for puntables
	FHitResult Hit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	DrawDebugLine(GetWorld(), Camera->GetComponentLocation(), Camera->GetComponentLocation() + Camera->GetForwardVector() * 2000, FColor::Red);
	if (BengalController->CanPunt() && GetWorld()->LineTraceSingleByChannel(Hit, Camera->GetComponentLocation(), Camera->GetComponentLocation() + Camera->GetForwardVector() * 2000, ECC_WorldStatic, QueryParams))
	{
		if (FVector::Dist(GetActorLocation(), Hit.ImpactPoint) > 400)
		{
			SetCurrentPuntable(nullptr);
		} else
		{
			if (ABreakableObject* Breakable = Cast<ABreakableObject>(Hit.GetActor()))
			{
				SetCurrentPuntable(Breakable);
				PuntableImpactPoint = Hit.ImpactPoint;
			} else SetCurrentPuntable(nullptr);
		}
	} else SetCurrentPuntable(nullptr);
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

void ABengalCharacter::SetCurrentPuntable(ABreakableObject* NewPuntable)
{
	if (HighlightedPuntable != nullptr)
		HighlightedPuntable->OutlineComp->HideOutline();
	HighlightedPuntable = NewPuntable;
	if (HighlightedPuntable)
		HighlightedPuntable->OutlineComp->ShowOutline();
}

void ABengalCharacter::DoPunt()
{
	if (HighlightedPuntable && PuntTimer <= 0 && !HighlightedPuntable->bHasBroken)
	{
		PuntTimer = 1;
		const FVector Direction = (PuntableImpactPoint - GetActorLocation()).GetSafeNormal();
		TargetYaw = Direction.Rotation().Yaw - 90;

		if (HighlightedPuntable->RequiredPuntLevel <= PuntLevel)
		{
			if (HighlightedPuntable->bStartStatic)
				HighlightedPuntable->Break();
			else
				HighlightedPuntable->MeshComp->AddImpulse(Direction * 300, NAME_None, true);
		}
	}
}

void ABengalCharacter::GiveXP(float Amount)
{
	UBengalGameInstance* BengalGI = Cast<UBengalGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	float NextLevelXP = BengalGI->ExperienceLevels[FMath::Clamp(PuntLevel - 1, 0, BengalGI->ExperienceLevels.Num() - 1)];
	float Needed = NextLevelXP - Experience;

	if (Amount < Needed)
		Experience += Amount;
	else
	{
		while (Amount > 0)
		{
			NextLevelXP = BengalGI->ExperienceLevels[FMath::Clamp(PuntLevel - 1, 0, BengalGI->ExperienceLevels.Num() - 1)];
			float XPUsed = FMath::Min(NextLevelXP, Amount);
			Amount -= XPUsed;
			AddPuntLevel();
			
			if (Amount <= 0)
			{
				Experience = XPUsed;
				Amount = 0;
			}
		}
	}
	
	Experience += Amount;
}

void ABengalCharacter::AddPuntLevel()
{
	// TODO: UI
	PuntLevel++;
}

void ABengalCharacter::PossessedBy(AController* NewController)
{
	BengalController = Cast<ABengalController>(NewController);
	Super::PossessedBy(NewController);
}
