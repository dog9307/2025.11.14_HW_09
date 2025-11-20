#include "ChatWidget.h"

#include "Components/EditableTextBox.h"
#include "Components/VerticalBox.h"
#include "Components/ScrollBox.h"

#include "../Player/BBPlayerController.h"
#include "../Player/BBPlayerState.h"
#include "TextLogBox.h"

#include "CommonMacros.h"

void UChatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextcommitted))
	{
		ChatInput->OnTextCommitted.AddDynamic(this, &ThisClass::OnChatInputTextcommitted);

		ChatInput->SetText(FText());
	}
}

void UChatWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (!ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextcommitted))
	{
		ChatInput->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnChatInputTextcommitted);
	}
}

void UChatWidget::AddLog(const FString& PlayerName, const FString& Log, int32 Style)
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

void UChatWidget::OnChatInputTextcommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		VALID_CHECK(APlayerController, PC, GetOwningPlayer(), );
		CAST_VALID_CHECK(ABBPlayerController, PlayerController, PC, );

		PlayerController->SetChatMessageString(Text.ToString());

		ChatInput->SetText(FText());
	}
}
