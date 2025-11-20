#include "ResultWidget.h"

#include "Components/VerticalBox.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

#include "TextLogBox.h"

void UResultWidget::AddLog(const FString& PlayerName, const FString& Log, int32 Style)
{
	UTextLogBox* LogBox = Super::CreateLogBox(Style);
	if (!IsValid(LogBox)) return;

	LogBox->SetInfo(PlayerName, Log);

	LogList->AddChildToVerticalBox(LogBox);
	FMargin Margin;
	Margin.Left = 10.0f;
	Margin.Right = 10.0f;
	Margin.Top = 10.0f;
	Margin.Bottom = 10.0f;
	LogBox->SetPadding(Margin);

	LogListScroll->ScrollToEnd();
}

void UResultWidget::SetPlayerInfo(const FString& Name, const int32& CurrentCount, const int32& MaxCount)
{
	if (!IsValid(PlayerNameTextBlock)) return;

	PlayerNameTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%s ( %d / %d )"), *Name, CurrentCount, MaxCount)));
}

void UResultWidget::UpdateTimeTextBlock(const FString& PlayerName, const float& LeftTime)
{
	if (!IsValid(TimeTextBlock)) return;

	TimeTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%s Turn ( %.0fs )"), *PlayerName, LeftTime)));
}
