#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TextLogBox.generated.h"

class UTextBlock;

UCLASS()
class BASEBALL_API UTextLogBox : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> PlayerNameTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> LogTextBlock;

public:
	void SetInfo(const FString& PlayerName, const FString& Log);
	void SetPlayerName(const FString& Name);
	void SetLog(const FString& Log);
};
