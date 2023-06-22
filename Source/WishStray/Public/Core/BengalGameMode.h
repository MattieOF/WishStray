// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BengalGameMode.generated.h"

// Struct defining a break sound entry
struct FBreakSoundQueueEntry
{
	float TimeUntilPlayed;
	USoundBase* Sound;
	FVector Location;
};

/**
 * Game Mode for Wish Stray
 */
UCLASS()
class WISHSTRAY_API ABengalGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABengalGameMode();

	virtual void Tick(float DeltaSeconds) override;
	
	void AddBreakSound(FBreakSoundQueueEntry SoundQueueEntry);
	void AddBreakSound(float MinTime, float MaxTime, USoundBase* Sound, FVector Location);
	void AddBreakSounds(int Count, float MinTime, float MaxTime, USoundBase* Sound, FVector Location);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetTotalItemWeight() { return TotalItemWeight; }

protected:
	UPROPERTY(BlueprintReadOnly)
	float TotalItemWeight;
	
private:
	TArray<FBreakSoundQueueEntry> BreakSoundQueue;
};
