// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PawPrint.generated.h"

UCLASS()
class WISHSTRAY_API APawPrint : public AActor
{
	GENERATED_BODY()

public:
	APawPrint();

	UFUNCTION(BlueprintCallable)
	void SetVisible(bool NewVisibility);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* LeftPaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* RightPaw;
};
