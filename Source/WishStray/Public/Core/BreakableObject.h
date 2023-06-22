// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BreakableObject.generated.h"

class UOutlineComponent;
class UGeometryCollection;
class UGeometryCollectionComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBroken);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPostOnBroken);

/**
 * An object that can be broken.
 */
UCLASS()
class WISHSTRAY_API ABreakableObject : public AActor
{
	GENERATED_BODY()

public:
	ABreakableObject();

	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void Break();

	UFUNCTION()
	void OnMeshHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BreakSpeed = 1000;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UGeometryCollection* GeoCollection;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* MeshComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UGeometryCollectionComponent* GeoCollectionComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UOutlineComponent* OutlineComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* BreakSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D SoundCount = FVector2D(1, 1);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D SoundDelayRange = FVector2D(0, 0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasBroken = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bStartStatic = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDebrisHaveCollision = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BreakAfterDistanceFallen = 1000000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int RequiredPuntLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float XPWorth = 0.15f;

	UPROPERTY(BlueprintAssignable)
	FOnBroken OnBroken;

	UPROPERTY(BlueprintAssignable)
	FPostOnBroken PostOnBroken;

private:
	float LastZPosition;
};
