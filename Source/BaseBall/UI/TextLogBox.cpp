#include "TextLogBox.h"

#include "Components/TextBlock.h"

void UTextLogBox::SetInfo(const FString& PlayerName, const FString& Log)
{
	SetPlayerName(PlayerName);
	SetLog(Log);
}

void UTextLogBox::SetPlayerName(const FString& PlayerName)
{
	if (!IsValid(PlayerNameTextBlock)) return;

	PlayerNameTextBlock->SetText(FText::FromString(PlayerName));
}

void UTextLogBox::SetLog(const FString& Log)
{
	if (!IsValid(LogTextBlock)) return;

	LogTextBlock->SetText(FText::FromString(Log));
}
