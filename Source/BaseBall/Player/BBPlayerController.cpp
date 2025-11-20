#include "BBPlayerController.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

#include "../UI/MainHUD.h"
#include "../Game/BBGameModeBase.h"
#include "BBPlayerState.h"

#include "CommonMacros.h"

ABBPlayerController::ABBPlayerController()
{
	bReplicates = true;
}

void ABBPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocalController()) return;

	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);
	SetShowMouseCursor(true);

	if (IsValid(MainHUDClass))
	{
		VALID_CHECK(UMainHUD, Temp, CreateWidget<UMainHUD>(this, MainHUDClass), );

		MainHUD = Temp;
		MainHUD->AddToViewport();
	}
}

void ABBPlayerController::SetChatMessageString(const FString& InChatMessageString)
{
	if (IsLocalController())
	{
		VALID_CHECK(ABBPlayerState, BBPS, GetPlayerState<ABBPlayerState>(), );

		ServerRPCPrintChatMessageString(InChatMessageString);
	}
}

void ABBPlayerController::PrintChatMessageString(const FString& PlayerName, const FString& InChatMessageString, int32 Style)
{
	if (!IsValid(MainHUD)) return;

	MainHUD->PrintChatMessageString(PlayerName, InChatMessageString, Style);
}

void ABBPlayerController::NotificationChatMessageString(const FString& InChatMessageString)
{
	if (!IsValid(MainHUD)) return;

	MainHUD->NotificationChatMessageString(InChatMessageString);
}

void ABBPlayerController::PrintResultMessageString(const FString& PlayerName, const FString& InResultString, int32 Style)
{
	if (!IsValid(MainHUD)) return;

	MainHUD->PrintResultString(PlayerName, InResultString, Style);
}

void ABBPlayerController::ClientRPCUpdateTime_Implementation(const FString& CurrentPlayerName, const float& LeftTime)
{
	if (!IsValid(MainHUD)) return;

	MainHUD->UpdateTime(CurrentPlayerName, LeftTime);
}

void ABBPlayerController::ClientRPCSetPlayerInfo_Implementation(const FString& PlayerName, const int32& CurrentCount, const int32& MaxCount)
{
	if (!IsValid(MainHUD)) return;

	MainHUD->SetPlayerInfo(PlayerName, CurrentCount, MaxCount);
}

void ABBPlayerController::ClientRPCResultString_Implementation(const FString& PlayerName, const FString& InChatMessageString, int32 Style)
{
	PrintResultMessageString(PlayerName, InChatMessageString, Style);
}

void ABBPlayerController::ClientRPCPrintChatMessageString_Implementation(const FString& PlayerName, const FString& InChatMessageString, int32 Style)
{
	PrintChatMessageString(PlayerName, InChatMessageString, Style);
}

void ABBPlayerController::ClientRPCNotificationMessageString_Implementation(const FString& InChatMessageString)
{
	PrintChatMessageString("SYSTEM", InChatMessageString, 2);
}

void ABBPlayerController::ServerRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	VALID_CHECK(AGameModeBase, GM, UGameplayStatics::GetGameMode(this), );
	CAST_VALID_CHECK(ABBGameModeBase, BBGM, GM, );

	BBGM->PrintChatMessageString(this, InChatMessageString);
}

void ABBPlayerController::ServerRPCSubmitAnswer_Implementation(const FString& InChatMessageString)
{
	VALID_CHECK(AGameModeBase, GM, UGameplayStatics::GetGameMode(this), );
	CAST_VALID_CHECK(ABBGameModeBase, BBGM, GM, );

	BBGM->PrintSubmitResult(this, InChatMessageString);
}
