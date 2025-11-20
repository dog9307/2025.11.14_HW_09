#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BBPlayerState.generated.h"

UCLASS()
class BASEBALL_API ABBPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ABBPlayerState();
	
protected:
	UPROPERTY(Replicated)
	FString PlayerName;

	UPROPERTY(Replicated)
	int32 CurrentGuessCount;

	UPROPERTY(Replicated)
	int32 MaxGuessCount;

public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	void InitPlayerState(int32 Count);
	void ResetPlay(int32 MaxGuess);
	void IncreaseGuessCount();

	bool IsCanPlay();

	FORCEINLINE void SetPlayerName(const FString& Name) { PlayerName = Name; }
	FORCEINLINE FString GetPlayerName() const { return PlayerName; }

	FORCEINLINE int32 GetCurrentGuessCount() const { return CurrentGuessCount; }
	FORCEINLINE int32 GetMaxGuessCount() const { return MaxGuessCount; }
};
