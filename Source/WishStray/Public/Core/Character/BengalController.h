// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BengalController.generated.h"

class USplineComponent;
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
	void OnPunt();

	FORCEINLINE bool CanPunt() { return !bJumping && !bChargingJump; }

protected:
	bool GetJumpToLocation(FVector& OutPos);
	
	UPROPERTY(BlueprintReadOnly)
	bool bChargingJump = false;

	UPROPERTY(BlueprintReadOnly)
	bool bJumping = false;

	UPROPERTY(BlueprintReadOnly)
	bool bHasJumpLocation = false;

	UPROPERTY(BlueprintReadOnly)
	float JumpProgress = 0;

	UPROPERTY(BlueprintReadOnly)
	float JumpCharge = 0;

	UPROPERTY()
	APawPrint* PawPrint;

	UPROPERTY(BlueprintReadWrite)
	FVector JumpPosOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USplineComponent* Spline;
};
