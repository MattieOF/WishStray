// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BengalController.generated.h"

UCLASS()
class WISHSTRAY_API ABengalController : public APlayerController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABengalController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
