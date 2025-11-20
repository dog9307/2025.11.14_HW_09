#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BBGameModeBase.generated.h"

class ABBPlayerController;

UCLASS()
class BASEBALL_API ABBGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	FString SecretNumberString;
	TArray<TObjectPtr<ABBPlayerController>> AllPlayerControllers;
	bool bIsGameStart;
	FTimerHandle StartGameTimer;

	const float TurnDuration = 30.0f;
	FTimerHandle TurnTimer;
	FTimerHandle TimeUpdateTimer;
	int32 CurrentTurnPlayerIdx;

public:
	virtual void BeginPlay() override; 
	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	FString GenerateSecretNumber();
	bool IsGuessNumberString(const FString& InNumberString);
	FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString);

	void PrintChatMessageString(ABBPlayerController* InChattingPlayerController, const FString& InChatMessageString);
	void PrintSubmitResult(ABBPlayerController* InSubmitPlayer, const FString& InAnswer);

	void IncreaseGuessCount(ABBPlayerController* InChattingPlayerController);

	void StartGame();
	void StartTurn();
	void EndTurn(int StrikeCount);
	void ResetGame();
	bool IsDraw();
	void JudgeGame(ABBPlayerController* InChattingPlayerController, int InStrikeCount);
};
