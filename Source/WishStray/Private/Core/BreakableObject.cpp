// copyright lololol

#include "Core/BreakableObject.h"

#include "WishStray.h"
#include "Core/BengalGameMode.h"
#include "Core/Character/BengalCharacter.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Visual/OutlineComponent.h"

ABreakableObject::ABreakableObject()
{
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	MeshComp->SetNotifyRigidBodyCollision(true);
	OutlineComp = CreateDefaultSubobject<UOutlineComponent>(TEXT("Outline"));
	RootComponent = MeshComp;
}

void ABreakableObject::BeginPlay()
{
	Super::BeginPlay();
	
	if (Mesh)
		MeshComp->SetStaticMesh(Mesh);
	MeshComp->SetUseCCD(!bStartStatic);
	MeshComp->SetSimulatePhysics(!bStartStatic);
	MeshComp->ComponentTags.Add("Outline");
	OutlineComp->RefreshOutlinedComponents();
	LastZPosition = MeshComp->GetComponentLocation().Z;
	
	FScriptDelegate MeshHitDelegate;
	MeshHitDelegate.BindUFunction(this, "OnMeshHit");
	MeshComp->OnComponentHit.Add(MeshHitDelegate); // BindDynamic causes breakpoint?
}

void ABreakableObject::Break()
{
	if (bHasBroken)
		return;

	OnBroken.Broadcast();

	Cast<ABengalCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->GiveXP(XPWorth);

	if (BreakSound)
	{
		Cast<ABengalGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->AddBreakSounds(
			FMath::RandRange(static_cast<int>(SoundCount.X), static_cast<int>(SoundCount.Y)),
			SoundDelayRange.X, SoundDelayRange.Y, BreakSound,
			GetActorLocation());
	}
	
	GeoCollectionComp = Cast<UGeometryCollectionComponent>(
		AddComponentByClass(UGeometryCollectionComponent::StaticClass(), false, FTransform(MeshComp->GetRelativeRotation(), FVector::ZeroVector, MeshComp->GetRelativeScale3D()), true));
	GeoCollectionComp->SetRestCollection(GeoCollection);
	GeoCollectionComp->SetCanEverAffectNavigation(false);
	FinishAddComponent(GeoCollectionComp, false, FTransform::Identity);
	GeoCollectionComp->CrumbleActiveClusters();

	MeshComp->DestroyComponent();
	RootComponent = GeoCollectionComp;

	PostOnBroken.Broadcast();

	bHasBroken = true;
}

void ABreakableObject::OnMeshHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (abs(LastZPosition - HitComponent->GetComponentLocation().Z) >= BreakAfterDistanceFallen
		|| (HitComponent->HasValidPhysicsState() && HitComponent->GetComponentVelocity().Length() >= BreakSpeed)
		|| (OtherComp && OtherComp->GetComponentVelocity().Length() >= BreakSpeed))
	{
		UE_LOG(LogBengal, Log, TEXT("Breakable object broken: this velocity: %f, collider velocity: %f"), HitComponent->GetComponentVelocity().Length(), OtherComp ? OtherComp->GetComponentVelocity().Length() : 0);
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
