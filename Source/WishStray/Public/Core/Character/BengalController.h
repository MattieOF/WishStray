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

	virtual void SetupInputComponent() override;

	void OnVerticalMovement(float Value);
	void OnHorizontalMovement(float Value);
	void OnVerticalLook(float Value);
	void OnHorizontalLook(float Value);

	void OnBeginJump();
	void OnEndJump();

protected:
	UPROPERTY(BlueprintReadOnly)
	bool bChargingJump = false;
};
