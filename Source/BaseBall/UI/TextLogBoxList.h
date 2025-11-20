#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TextLogBoxList.generated.h"

class UVerticalBox;
class UScrollBox;
class UTextLogBox;

UCLASS(Abstract)
class BASEBALL_API UTextLogBoxList : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UTextLogBox>> LogBoxSytleClasses;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> LogList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> LogListScroll;

public:
	virtual UTextLogBox* CreateLogBox(int32 Style);
	virtual void AddLog (const FString& PlayerName, const FString& Log, int32 Style) PURE_VIRTUAL(ThisClass::AddLog, );
};
