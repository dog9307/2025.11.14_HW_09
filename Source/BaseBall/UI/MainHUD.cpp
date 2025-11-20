#include "MainHUD.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

#include "Player/BBPlayerController.h"

#include "ChatWidget.h"
#include "ResultWidget.h"
#include "NumButton.h"

#include "CommonMacros.h"

void UMainHUD::NativeConstruct()
{
	Super::NativeConstruct();

	if (!ResultButton->OnClicked.IsAlreadyBound(this, &ThisClass::OnClickResultButton))
	{
		ResultButton->OnClicked.AddDynamic(this, &ThisClass::OnClickResultButton);
	}

	ResetGame();
}

void UMainHUD::NativeDestruct()
{
	Super::NativeDestruct();

	if (!ResultButton->OnClicked.IsAlreadyBound(this, &ThisClass::OnClickResultButton))
	{
		ResultButton->OnClicked.RemoveDynamic(this, &ThisClass::OnClickResultButton);
	}
}

void UMainHUD::NotificationChatMessageString(const FString& InChatMessageString)
{
	if (!IsValid(ChatWidget)) return;

	ChatWidget->AddLog("SYSTEM", InChatMessageString, 2);
}

void UMainHUD::PrintChatMessageString(const FString& PlayerName, const FString& InChatMessageString, int32 Style)
{
	if (!IsValid(ChatWidget)) return;

	ChatWidget->AddLog(PlayerName, InChatMessageString, Style);
}

void UMainHUD::PrintResultString(const FString& PlayerName, const FString& InChatMessageString, int32 Style)
{
	if (!IsValid(ResultWidget)) return;

	ResultWidget->AddLog(PlayerName, InChatMessageString, Style);
}

void UMainHUD::SetPlayerInfo(const FString& PlayerName, const int32& CurrentCount, const int32& MaxCount)
{
	if (!IsValid(ResultWidget)) return;

	ResultWidget->SetPlayerInfo(PlayerName, CurrentCount, MaxCount);
}

void UMainHUD::UpdateTime(const FString& CurrentPlayer, const float& LeftTime)
{
	if (!IsValid(ResultWidget)) return;

	ResultWidget->UpdateTimeTextBlock(CurrentPlayer, LeftTime);
}

void UMainHUD::OnClickResultButton()
{
	CAST_VALID_CHECK(ABBPlayerController, BBPC, UGameplayStatics::GetPlayerController(GetWorld(), 0), );

	FString Answer = FString::Printf(TEXT("%d%d%d"), Num_0->GetNum(), Num_1->GetNum(), Num_2->GetNum());
	BBPC->ServerRPCSubmitAnswer(Answer);
}

void UMainHUD::ResetGame()
{
	if (!IsValid(Num_0)) return;
	if (!IsValid(Num_1)) return;
	if (!IsValid(Num_2)) return;
	if (!IsValid(ResultButton)) return;

	Num_0->ResetNum();
	Num_1->ResetNum();
	Num_2->ResetNum();
}
