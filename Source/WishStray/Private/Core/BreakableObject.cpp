// copyright lololol

#include "Core/BreakableObject.h"

#include "WishStray.h"
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
	MeshComp->SetUseCCD(!bStartStatic);
	MeshComp->SetSimulatePhysics(!bStartStatic);
	LastZPosition = MeshComp->GetComponentLocation().Z;
	
	FScriptDelegate MeshHitDelegate;
	MeshHitDelegate.BindUFunction(this, "OnMeshHit");
	MeshComp->OnComponentHit.Add(MeshHitDelegate); // BindDynamic causes breakpoint?
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
	if (abs(LastZPosition - HitComponent->GetComponentLocation().Z) >= BreakAfterDistanceFallen
		|| (HitComponent->HasValidPhysicsState() && HitComponent->GetComponentVelocity().Length() >= BreakSpeed)
		|| (OtherComp && OtherComp->GetComponentVelocity().Length() >= BreakSpeed))
	{
		UE_LOG(LogBengal, Log, TEXT("Breakable object broken: this velocity: %f, collider velocity: %f"), HitComponent->GetComponentVelocity().Length(), OtherComp->GetComponentVelocity().Length());
		Break();
	}
	
	LastZPosition = HitComponent->GetComponentLocation().Z;
}

#if WITH_EDITOR
void ABreakableObject::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (!PropertyChangedEvent.Property)
		return;
	
	if (PropertyChangedEvent.Property->GetNameCPP() == "Mesh" && MeshComp)
		MeshComp->SetStaticMesh(Mesh);
}
#endif
