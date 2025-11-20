#include "BBGameModeBase.h"

#include "EngineUtils.h"

#include "../Player/BBPlayerController.h"
#include "../Player/BBPlayerState.h"

#include "BBGameStateBase.h"

#include "CommonMacros.h"

void ABBGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	bIsGameStart = false;
}

void ABBGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	CAST_VALID_CHECK(ABBPlayerController, BBPC, NewPlayer, );
	VALID_CHECK(ABBPlayerState, BBPS, BBPC->GetPlayerState<ABBPlayerState>(), );

	BBPS->SetPlayerName(TEXT("Player") + FString::FromInt(AllPlayerControllers.Num() + 1));


	VALID_CHECK(ABBGameStateBase, BBGS, GetGameState<ABBGameStateBase>(), );

	BBGS->MulticastRPCBroadcastLoginMessage(BBPS->GetPlayerName());


	AllPlayerControllers.Add(BBPC);

	if (AllPlayerControllers.Num() == 2)
	{
		StartGame();
	}
}

void ABBGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearAllTimersForObject(this);

	Super::EndPlay(EndPlayReason);
}

FString ABBGameModeBase::GenerateSecretNumber()
{
	TArray<int32> Numbers;
	for (int32 i = 1; i <= 9; ++i)
	{
		Numbers.Add(i);
	}

	FMath::RandInit(FDateTime::Now().GetTicks());
	Numbers = Numbers.FilterByPredicate([](int32 Num) { return Num > 0; });

	FString Result;
	for (int32 i = 0; i < 3; ++i)
	{
		int32 Index = FMath::RandRange(0, Numbers.Num() - 1);
		Result.Append(FString::FromInt(Numbers[Index]));
		Numbers.RemoveAt(Index);
	}

	UE_LOG(LogTemp, Error, TEXT("%s"), *Result);

	return Result;
}

bool ABBGameModeBase::IsGuessNumberString(const FString& InNumberString)
{
	bool bCanPlay = false;

	do {

		if (InNumberString.Len() != 3)
		{
			break;
		}

		bool bIsUnique = true;
		TSet<TCHAR> UniqueDigits;
		for (TCHAR C : InNumberString)
		{
			if (FChar::IsDigit(C) == false)
			{
				bIsUnique = false;
				break;
			}

			int32 prevSize = UniqueDigits.Num();

			UniqueDigits.Add(C);
			if (UniqueDigits.Num() == prevSize)
			{
				bIsUnique = false;
				break;
			}
		}

		if (bIsUnique == false)
		{
			break;
		}

		bCanPlay = true;

	} while (false);

	return bCanPlay;
}

FString ABBGameModeBase::JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString)
{
	int32 StrikeCount = 0, BallCount = 0;

	for (int32 i = 0; i < 3; ++i)
	{
		if (InSecretNumberString[i] == InGuessNumberString[i])
		{
			StrikeCount++;
		}
		else
		{
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), InGuessNumberString[i]);
			if (InSecretNumberString.Contains(PlayerGuessChar))
			{
				BallCount++;
			}
		}
	}

	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}

	return FString::Printf(TEXT("%dS%dB"), StrikeCount, BallCount);
}

void ABBGameModeBase::PrintChatMessageString(ABBPlayerController* InChattingPlayerController, const FString& InChatMessageString)
{
	VALID_CHECK(ABBPlayerState, BBPS, InChattingPlayerController->GetPlayerState<ABBPlayerState>(), );

	for (TActorIterator<ABBPlayerController> It(GetWorld()); It; ++It)
	{
		ABBPlayerController* BBPlayerController = *It;
		if (IsValid(BBPlayerController) == true)
		{
			int32 Style = (InChattingPlayerController == BBPlayerController ? 0 : 1);
			BBPlayerController->ClientRPCPrintChatMessageString(BBPS->GetPlayerName(), InChatMessageString, Style);
		}
	}
}

void ABBGameModeBase::PrintSubmitResult(ABBPlayerController* InSubmitPlayer, const FString& InAnswer)
{
	if (InSubmitPlayer != AllPlayerControllers[CurrentTurnPlayerIdx])
	{
		InSubmitPlayer->ClientRPCNotificationMessageString("It's Not Your Turn. Please Wait your Turn.");
		return;
	}

	VALID_CHECK(ABBPlayerState, BBPS, InSubmitPlayer->GetPlayerState<ABBPlayerState>(), );

	if (BBPS->IsCanPlay())
	{
		if (IsGuessNumberString(InAnswer))
		{
			FString JudgeResultString = JudgeResult(SecretNumberString, InAnswer);

			IncreaseGuessCount(InSubmitPlayer);

			for (TActorIterator<ABBPlayerController> It(GetWorld()); It; ++It)
			{
				ABBPlayerController* BBPlayerController = *It;
				if (IsValid(BBPlayerController) == true)
				{
					FString CombinedMessageString = InAnswer + TEXT("  ->  ") + JudgeResultString;
					int32 Style = (InSubmitPlayer == BBPlayerController ? 0 : 1);
					BBPlayerController->ClientRPCResultString(BBPS->GetPlayerName(), CombinedMessageString, Style);
				}
			}


			int32 StrikeCount = FCString::Atoi(*JudgeResultString.Left(1));
			EndTurn(StrikeCount);
			//JudgeGame(InSubmitPlayer, StrikeCount);
		}
		else
		{
			InSubmitPlayer->ClientRPCNotificationMessageString("You Submit Incorrect Format Answer.");
		}
	}
	else
	{
		InSubmitPlayer->ClientRPCNotificationMessageString("You Don't Have Any Chance.");
	}
}

void ABBGameModeBase::IncreaseGuessCount(ABBPlayerController* InChattingPlayerController)
{
	VALID_CHECK(ABBPlayerState, BBPS, InChattingPlayerController->GetPlayerState<ABBPlayerState>(), );

	BBPS->IncreaseGuessCount();
	InChattingPlayerController->ClientRPCSetPlayerInfo(BBPS->GetPlayerName(), BBPS->GetCurrentGuessCount(), BBPS->GetMaxGuessCount());
}

void ABBGameModeBase::StartGame()
{
	GetWorldTimerManager().ClearTimer(TimeUpdateTimer);
	GetWorldTimerManager().ClearTimer(TurnTimer);
	GetWorldTimerManager().ClearTimer(StartGameTimer);
	GetWorldTimerManager().SetTimer(
		StartGameTimer,
		[this]()
		{
			bIsGameStart = true;

			ResetGame();

			for (const auto& BBPlayerController : AllPlayerControllers)
			{
				BBPlayerController->ClientRPCResultString("SYSTEM", "New Game Start", 2);
			}

			CurrentTurnPlayerIdx = 0;
			StartTurn();
		},
		3.0f,
		false
	);
}

void ABBGameModeBase::StartTurn()
{
	GetWorldTimerManager().ClearTimer(TurnTimer);
	GetWorldTimerManager().SetTimer(
		TurnTimer,
		[this]()
		{
			EndTurn(0);
		},
		TurnDuration,
		false
	);

	GetWorldTimerManager().ClearTimer(TimeUpdateTimer);
	GetWorldTimerManager().SetTimer(
		TimeUpdateTimer,
		[this]()
		{
			ABBPlayerState* BBPS = AllPlayerControllers[CurrentTurnPlayerIdx]->GetPlayerState<ABBPlayerState>();
			if (IsValid(BBPS))
			{
				for (const auto& BBPlayerController : AllPlayerControllers)
				{
					BBPlayerController->ClientRPCUpdateTime(BBPS->GetPlayerName(), GetWorldTimerManager().GetTimerRemaining(TurnTimer));
				}
			}
		},
		1.0f,
		true
	);
}

void ABBGameModeBase::EndTurn(int StrikeCount)
{
	if (StrikeCount == 3)
	{
		ABBPlayerState* BBPS = AllPlayerControllers[CurrentTurnPlayerIdx]->GetPlayerState<ABBPlayerState>();
		for (const auto& BBPlayerController : AllPlayerControllers)
		{
			if (IsValid(BBPS))
			{
				FString CombinedMessageString = BBPS->GetPlayerName() + TEXT(" has won the game.");
				BBPlayerController->ClientRPCResultString("SYSTEM", CombinedMessageString, 2);
			}
		}

		bIsGameStart = false;

		StartGame();
	}
	else
	{
		if (IsDraw())
		{
			for (const auto& BBPlayerController : AllPlayerControllers)
			{
				BBPlayerController->ClientRPCResultString("SYSTEM", "Draw...", 2);
			}

			bIsGameStart = false;

			StartGame();
		}
		else
		{
			CurrentTurnPlayerIdx = (CurrentTurnPlayerIdx + 1) % 2;
			StartTurn();
		}
	}
}

void ABBGameModeBase::ResetGame()
{
	SecretNumberString = GenerateSecretNumber();

	for (int32 i = 0; i < AllPlayerControllers.Num(); ++i)
	{
		VALID_CHECK(ABBPlayerState, BBPS, AllPlayerControllers[i]->GetPlayerState<ABBPlayerState>(), );

		BBPS->ResetPlay(i == 0 ? 4 : 3);
		AllPlayerControllers[i]->ClientRPCSetPlayerInfo(BBPS->GetPlayerName(), BBPS->GetCurrentGuessCount(), BBPS->GetMaxGuessCount());
	}
}

bool ABBGameModeBase::IsDraw()
{
	bool bIsDraw = true;
	for (const auto& BBPlayerController : AllPlayerControllers)
	{
		ABBPlayerState* BBPS = BBPlayerController->GetPlayerState<ABBPlayerState>();
		if (IsValid(BBPS) == true)
		{
			if (BBPS->IsCanPlay())
			{
				bIsDraw = false;
				break;
			}
		}
	}

	return bIsDraw;
}

void ABBGameModeBase::JudgeGame(ABBPlayerController* InChattingPlayerController, int InStrikeCount)
{
	if (3 == InStrikeCount)
	{
		ABBPlayerState* BBPS = InChattingPlayerController->GetPlayerState<ABBPlayerState>();
		for (const auto& BBPlayerController : AllPlayerControllers)
		{
			if (IsValid(BBPS))
			{
				FString CombinedMessageString = BBPS->GetPlayerName() + TEXT(" has won the game.");
				BBPlayerController->ClientRPCNotificationMessageString(CombinedMessageString);
			}
		}

		bIsGameStart = false;

		StartGame();
	}
	else
	{
		if (IsDraw())
		{
			for (const auto& BBPlayerController : AllPlayerControllers)
			{
				BBPlayerController->ClientRPCResultString("SYSTEM", "Draw...", 2);
			}

			bIsGameStart = false;

			StartGame();
		}
	}
}
