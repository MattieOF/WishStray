// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameModeBase.h"
#include "BengalGameMode.generated.h"

// Struct defining a break sound entry
struct FBreakSoundQueueEntry
{
	float TimeUntilPlayed;
	USoundBase* Sound;
	FVector Location;
};

UCLASS(Blueprintable)
class UNotificationPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void AddNotification(const FText& Title, const FText& Content, float Time = 5, UTexture2D* Icon = nullptr);
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

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	void AddBreakSound(FBreakSoundQueueEntry SoundQueueEntry);
	void AddBreakSound(float MinTime, float MaxTime, USoundBase* Sound, FVector Location);
	void AddBreakSounds(int Count, float MinTime, float MaxTime, USoundBase* Sound, FVector Location);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetTotalItemWeight() { return TotalItemWeight; }

	UPROPERTY(EditAnywhere)
	TSubclassOf<UNotificationPanel> NotificationsWidgetClass;
	
	UPROPERTY(BlueprintReadOnly)
	UNotificationPanel* NotificationPanel;

protected:
	UPROPERTY(BlueprintReadOnly)
	float TotalItemWeight;
	
private:
	TArray<FBreakSoundQueueEntry> BreakSoundQueue;
};
