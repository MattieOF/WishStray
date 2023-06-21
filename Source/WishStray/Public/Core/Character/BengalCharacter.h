// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "BengalCharacter.generated.h"

class ABengalController;
class ABreakableObject;

UCLASS()
class WISHSTRAY_API ABengalCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABengalCharacter();

	virtual void Tick(float DeltaSeconds) override;

	virtual void AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce) override;
	
	UFUNCTION(BlueprintCallable)
	void AddCameraInput(float Vertical, float Horizontal);

	UFUNCTION(BlueprintCallable)
	void SetCurrentPuntable(ABreakableObject* NewPuntable);

	UFUNCTION(BlueprintCallable)
	void DoPunt();
	
	virtual void PossessedBy(AController* NewController) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed = 800;

	UPROPERTY()
	ABreakableObject* HighlightedPuntable;

	UPROPERTY(BlueprintReadOnly)
	FVector PuntableImpactPoint;

	UPROPERTY(BlueprintReadWrite)
	float Experience = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int PuntLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PuntTimer = 0;
	
	UPROPERTY(BlueprintReadWrite)
	float TargetYaw = 0;

private:
	UPROPERTY()
	ABengalController* BengalController;
};
