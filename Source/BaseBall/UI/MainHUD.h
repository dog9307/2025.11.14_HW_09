#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHUD.generated.h"

class UChatWidget;
class UResultWidget;
class UNumButton;
class UButton;
class UTextBlock;

UCLASS()
class BASEBALL_API UMainHUD : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UChatWidget> ChatWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UResultWidget> ResultWidget;

	// 난이도 조절용으로 TArray로 잡고 따로 또 패널 만들고
	// 난이도에 따라 버튼 갯수 조절하고 하면 재밌겠지만
	// 할게 많으니 나중에 해보던가 하고 일단 패스.
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UNumButton> Num_0;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UNumButton> Num_1;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UNumButton> Num_2;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ResultButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ResultTextBlock;

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void NotificationChatMessageString(const FString& InChatMessageString);
	void PrintChatMessageString(const FString& PlayerName, const FString& InChatMessageString, int32 Style);
	void PrintResultString(const FString& PlayerName, const FString& InChatMessageString, int32 Style);

	void SetPlayerInfo(const FString& PlayerName, const int32& CurrentCount, const int32& MaxCount);
	void UpdateTime(const FString& CurrentPlayer, const float& LeftTime);

	UFUNCTION()
	void OnClickResultButton();
	void ResetGame();
};
