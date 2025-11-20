#include "BBPlayerState.h"

#include "Net/UnrealNetwork.h"

ABBPlayerState::ABBPlayerState()
{
	bReplicates = true;
}

void ABBPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PlayerName);
	DOREPLIFETIME(ThisClass, CurrentGuessCount);
	DOREPLIFETIME(ThisClass, MaxGuessCount);
}

void ABBPlayerState::InitPlayerState(int32 Count)
{
	MaxGuessCount = Count;
}

void ABBPlayerState::ResetPlay(int32 MaxGuess)
{
	CurrentGuessCount = 0;
	InitPlayerState(MaxGuess);
}

void ABBPlayerState::IncreaseGuessCount()
{
	++CurrentGuessCount;
}

bool ABBPlayerState::IsCanPlay()
{
	return CurrentGuessCount < MaxGuessCount;
}
