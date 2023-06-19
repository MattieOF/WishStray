// copyright lololol

#include "Core/Character/BengalPawn.h"

// Sets default values
ABengalPawn::ABengalPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABengalPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABengalPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
