// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BengalController.generated.h"

class APawPrint;

UCLASS()
class WISHSTRAY_API ABengalController : public APlayerController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABengalController();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupInputComponent() override;

	void OnVerticalMovement(float Value);
	void OnHorizontalMovement(float Value);
	void OnVerticalLook(float Value);
	void OnHorizontalLook(float Value);
	void OnScroll(float Value);

	void OnBeginJump();
	void OnEndJump();

protected:
	bool GetJumpToLocation(FVector& OutPos);
	
	UPROPERTY(BlueprintReadOnly)
	bool bChargingJump = false;

	UPROPERTY(BlueprintReadOnly)
	float JumpCharge = 0;

	UPROPERTY()
	APawPrint* PawPrint;

	UPROPERTY(BlueprintReadWrite)
	FVector JumpPosOffset;
};
