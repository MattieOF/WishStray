// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "BengalCharacter.generated.h"

UCLASS()
class WISHSTRAY_API ABengalCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABengalCharacter();

	virtual void AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce) override;
	
	UFUNCTION(BlueprintCallable)
	void AddCameraInput(float Vertical, float Horizontal);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed = 500;
};
