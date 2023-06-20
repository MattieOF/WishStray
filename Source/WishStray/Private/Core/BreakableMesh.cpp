// copyright lololol

#include "Core/BreakableMesh.h"

#include "WishStray.h"
#include "Core/BengalGameInstance.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Kismet/GameplayStatics.h"

ABreakableMesh::ABreakableMesh()
{
	PrimaryActorTick.bCanEverTick = true;
	
	GeoCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Geometry Collection"));
	GeoCollection->SetVisibility(false);
	GeoCollection->SetNotifyBreaks(true);
	GeoCollection->OnChaosBreakEvent.AddDynamic(this, &ABreakableMesh::OnMeshBreaks);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetSimulatePhysics(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetupAttachment(GeoCollection);

	RootComponent = GeoCollection;
}

void ABreakableMesh::BeginPlay()
{
	Super::BeginPlay();

	if (GeoCollection)
		GeoCollection->SetRestCollection(GeometryCollection);
	else
		UE_LOG(LogBengal, Error, TEXT("In %s, null geo collection!"), *GetDebugName(this));
	
	if (NormalMesh)
	{
		Mesh->SetStaticMesh(NormalMesh);
		PrimaryActorTick.bCanEverTick = true;
	}
	else
	{
		GeoCollection->SetVisibility(true);
		Mesh->DestroyComponent();
		PrimaryActorTick.bCanEverTick = false;
	}
}

void ABreakableMesh::Tick(float DeltaSeconds)
{
	if (Mesh && GeoCollection)
	{
		const FTransform Transform = GeoCollection->GetTransformArray()[0];
		Mesh->SetRelativeLocation(Transform.GetLocation());
		Mesh->SetRelativeRotation(Transform.GetRotation());
	}
	else
		PrimaryActorTick.bCanEverTick = false;
	
	Super::Tick(DeltaSeconds);
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
	GeoCollection->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	AActor* MF = GetWorld()->SpawnActor(static_cast<UBengalGameInstance*>(GetGameInstance())->MasterFieldActorClass, &SpawnPos, &SpawnRot);
	float Radius = Mesh->Bounds.SphereRadius;
	MF->ProcessEvent(MF->FindFunction(TEXT("SetRadius")), &Radius);
	MF->SetLifeSpan(0.05f);
	
	bHasBroken = true;
	
	Mesh->DestroyComponent();
}

void ABreakableMesh::OnMeshBreaks(const FChaosBreakEvent& BreakEvent)
{
	if (BreakSound)
	{	
		const auto& Transform = GeoCollection->GetTransformArray()[0];
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), BreakSound, GetActorLocation() + Transform.GetLocation(), GetActorRotation() + Transform.Rotator());
	}
	
	GeoCollection->SetVisibility(true);
	if (Mesh)
		Mesh->DestroyComponent();
	PrimaryActorTick.bCanEverTick = false; 
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

void ABreakableMesh::PrintFirstTransform()
{
	UE_LOG(LogBengal, Log, TEXT("%s"), *GeoCollection->GetTransformArray()[0].ToString());
}
#endif
