// copyright lololol

#include "Core/BreakableObject.h"

#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Kismet/GameplayStatics.h"

ABreakableObject::ABreakableObject()
{
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	MeshComp->SetNotifyRigidBodyCollision(true);
	RootComponent = MeshComp;
}

void ABreakableObject::BeginPlay()
{
	Super::BeginPlay();

	if (Mesh)
		MeshComp->SetStaticMesh(Mesh);

	MeshComp->OnComponentHit.AddDynamic(this, &ABreakableObject::OnMeshHit);
}

void ABreakableObject::Break()
{
	if (bHasBroken)
		return;

	if (BreakSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), BreakSound, MeshComp->GetComponentLocation(), MeshComp->GetComponentRotation());

	GeoCollectionComp = Cast<UGeometryCollectionComponent>(
		AddComponentByClass(UGeometryCollectionComponent::StaticClass(), false, FTransform::Identity, true));
	GeoCollectionComp->SetRestCollection(GeoCollection);
	FinishAddComponent(GeoCollectionComp, false, FTransform::Identity);
	GeoCollectionComp->CrumbleActiveClusters();

	MeshComp->DestroyComponent();
	RootComponent = GeoCollectionComp;
	
	bHasBroken = true;
}

void ABreakableObject::OnMeshHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (HitComponent->GetComponentVelocity().Length() >= BreakSpeed || OtherComp->GetComponentVelocity().Length() >= BreakSpeed)
		Break();
}

#if WITH_EDITOR
void ABreakableObject::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property->GetNameCPP() == "Mesh")
		MeshComp->SetStaticMesh(Mesh);
	
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
