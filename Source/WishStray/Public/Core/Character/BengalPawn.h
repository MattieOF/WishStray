// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BengalPawn.generated.h"

UCLASS()
class WISHSTRAY_API ABengalPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABengalPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
