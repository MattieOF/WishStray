// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BreakableObject.generated.h"

class UGeometryCollection;
class UGeometryCollectionComponent;

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
	USoundBase* BreakSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasBroken = false;
};
