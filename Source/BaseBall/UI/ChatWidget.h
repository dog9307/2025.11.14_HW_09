#pragma once

#include "CoreMinimal.h"
#include "TextLogBoxList.h"

#include "Types/SlateEnums.h"

#include "ChatWidget.generated.h"

class UEditableTextBox;

UCLASS()
class BASEBALL_API UChatWidget : public UTextLogBoxList
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> ChatInput;

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
public:
	virtual void AddLog(const FString& PlayerName, const FString& Log, int32 Style) override;

	UFUNCTION()
	void OnChatInputTextcommitted(const FText& Text, ETextCommit::Type CommitMethod);
};
