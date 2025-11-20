#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NumButton.generated.h"

class UTextBlock;
class UButton;

UCLASS()
class BASEBALL_API UNumButton : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> NumText;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> NumButton;

	UPROPERTY()
	int32 CurrentNum;

	const static int32 MIN = 1;
	const static int32 MAX = 9;

private:
	UFUNCTION()
	void SetNumText();

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	UFUNCTION()
	void IncreaseNum();
	UFUNCTION(BlueprintCallable)
	void DecreaseNum();
	UFUNCTION()
	void ResetNum();

	int32 GetNum();
};
