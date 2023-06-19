// copyright lololol

#include "Core/Character/BengalController.h"

// Sets default values
ABengalController::ABengalController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABengalController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABengalController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

