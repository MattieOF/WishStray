﻿// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GeometryCollection/GeometryCollectionObject.h"
#include "BreakableMesh.generated.h"

class UGeometryCollectionComponent;

UCLASS()
class WISHSTRAY_API ABreakableMesh : public AActor
{
	GENERATED_BODY()

public:
	ABreakableMesh();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* NormalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UGeometryCollection* GeometryCollection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RequiredVelocityToBreak = 1000;
	
	UPROPERTY(BlueprintReadWrite)
	bool bHasBroken = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UGeometryCollectionComponent* GeoCollection;

private:
	UFUNCTION(BlueprintCallable)
	void OnMeshHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};