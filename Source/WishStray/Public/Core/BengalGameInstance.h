// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BengalGameInstance.generated.h"

/**
 * Game instance for wish.com stray
 */
UCLASS()
class WISHSTRAY_API UBengalGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> MasterFieldActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<float> ExperienceLevels;
};
