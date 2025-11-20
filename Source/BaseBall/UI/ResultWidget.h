#pragma once

#include "CoreMinimal.h"
#include "TextLogBoxList.h"
#include "ResultWidget.generated.h"

class UTextBlock;

UCLASS()
class BASEBALL_API UResultWidget : public UTextLogBoxList
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> PlayerNameTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> TimeTextBlock;

public:
	virtual void AddLog(const FString& PlayerName, const FString& Log, int32 Style) override;

	void SetPlayerInfo(const FString& PlayerName, const int32& CurrentCount, const int32& MaxCount);
	void UpdateTimeTextBlock(const FString& PlayerName, const float& LeftTime);
};
