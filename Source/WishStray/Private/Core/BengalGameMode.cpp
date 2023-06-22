// copyright lololol

#include "Core/BengalGameMode.h"

#include "WishStray.h"
#include "Core/BreakableObject.h"
#include "Core/Character/BengalController.h"
#include "Core/Character/BengalCharacter.h"
#include "Kismet/GameplayStatics.h"

ABengalGameMode::ABengalGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	
	DefaultPawnClass      = ABengalCharacter::StaticClass();
	PlayerControllerClass = ABengalController::StaticClass();
}

void ABengalGameMode::BeginPlay()
{
	Super::BeginPlay();
	NotificationPanel = CreateWidget<UNotificationPanel>(GetWorld(), NotificationsWidgetClass);
	if (!NotificationPanel)
	{
    	UE_LOG(LogBengal, Error, TEXT("Failed to create notification panel!"));
	}
	else
    	NotificationPanel->AddToViewport();
}

void ABengalGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (TotalItemWeight == 0)
	{
		TArray<AActor*> Breakables;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABreakableObject::StaticClass(), Breakables);
		for (AActor* Breakable : Breakables)
			TotalItemWeight += Cast<ABreakableObject>(Breakable)->XPWorth;
	}
	
	for (int i = BreakSoundQueue.Num() - 1; i >= 0; i--)
	{
		BreakSoundQueue[i].TimeUntilPlayed -= DeltaSeconds;
		if (BreakSoundQueue[i].TimeUntilPlayed <= 0)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), BreakSoundQueue[i].Sound, BreakSoundQueue[i].Location, FRotator::ZeroRotator);
			BreakSoundQueue.RemoveAt(i);		
		}
	}
}

void ABengalGameMode::AddBreakSound(FBreakSoundQueueEntry SoundQueueEntry)
{
	BreakSoundQueue.Add(SoundQueueEntry);
}

void ABengalGameMode::AddBreakSound(float MinTime, float MaxTime, USoundBase* Sound, FVector Location)
{
	BreakSoundQueue.Add({ FMath::RandRange(MinTime, MaxTime), Sound, Location });
}

void ABengalGameMode::AddBreakSounds(int Count, float MinTime, float MaxTime, USoundBase* Sound, FVector Location)
{
	for (int i = 0; i < Count; i++)
		BreakSoundQueue.Add({ FMath::RandRange(MinTime, MaxTime), Sound, Location });
}
