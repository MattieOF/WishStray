// copyright lololol

#include "PawPrint.h"

#include "UObject/ConstructorHelpers.h"

// Sets default values
APawPrint::APawPrint()
{
	PrimaryActorTick.bCanEverTick = false;

	const auto MeshFinder = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/Meshes/SM_PawPrint.SM_PawPrint'"));
	
	LeftPaw  = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left Paw"));
	RightPaw = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Paw"));
	LeftPaw->SetStaticMesh(MeshFinder.Object);
	RightPaw->SetStaticMesh(MeshFinder.Object);
	LeftPaw->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightPaw->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftPaw->SetMobility(EComponentMobility::Movable);
	RightPaw->SetMobility(EComponentMobility::Movable);
	RootComponent = LeftPaw;
	RightPaw->SetupAttachment(LeftPaw);
	LeftPaw->SetRelativeLocation(FVector(-75, 0, 0));
	RightPaw->SetRelativeLocation(FVector(75, 0, 0));
	SetVisible(false);
}

void APawPrint::SetVisible(bool NewVisibility)
{
	LeftPaw->SetVisibility(NewVisibility);
	RightPaw->SetVisibility(NewVisibility);
}
