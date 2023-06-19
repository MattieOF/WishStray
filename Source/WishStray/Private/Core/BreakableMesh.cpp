// copyright lololol

#include "BreakableMesh.h"

#include "WishStray.h"
#include "Core/BengalGameInstance.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

ABreakableMesh::ABreakableMesh()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	GeoCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Geometry Collection"));
	GeoCollection->SetVisibility(false);
	GeoCollection->SetupAttachment(Mesh);

	FScriptDelegate OnMeshHitDelegate;
	OnMeshHitDelegate.BindUFunction(this, "OnMeshHit");
	Mesh->OnComponentHit.Add(OnMeshHitDelegate);
	Mesh->SetSimulatePhysics(true);
	Mesh->SetNotifyRigidBodyCollision(true);
}

void ABreakableMesh::BeginPlay()
{
	Super::BeginPlay();
	Mesh->SetStaticMesh(NormalMesh);
	GeoCollection->SetRestCollection(GeometryCollection);
}

void ABreakableMesh::OnMeshHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogBengal, Log, TEXT("Breakable object hit with velocity: %f"), Mesh->GetComponentVelocity().Length());
	
	if (bHasBroken || static_cast<float>(Mesh->GetComponentVelocity().Length()) < RequiredVelocityToBreak)
		return;
	
	const FVector  SpawnPos = Mesh->GetComponentLocation();
	const FRotator SpawnRot = Mesh->GetComponentRotation();
	GeoCollection->SetVisibility(true);
	AActor* MF = GetWorld()->SpawnActor(static_cast<UBengalGameInstance*>(GetGameInstance())->MasterFieldActorClass, &SpawnPos, &SpawnRot);
	float Radius = Mesh->Bounds.SphereRadius;
	MF->ProcessEvent(MF->FindFunction(TEXT("SetRadius")), &Radius);
	MF->SetLifeSpan(0.05f);
	
	bHasBroken = true;
	
	Mesh->DestroyComponent();
}

#if WITH_EDITOR
void ABreakableMesh::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property->GetName() == "NormalMesh")
		Mesh->SetStaticMesh(NormalMesh);
	else if (PropertyChangedEvent.Property->GetName() == "GeometryCollection")
		GeoCollection->SetRestCollection(GeometryCollection);
	
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
