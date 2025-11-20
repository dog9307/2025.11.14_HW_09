#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BBPlayerController.generated.h"

class UMainHUD;

UCLASS()
class BASEBALL_API ABBPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABBPlayerController();

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMainHUD> MainHUDClass;

	UPROPERTY()
	TObjectPtr<UMainHUD> MainHUD;

public:
	virtual void BeginPlay() override;

	void SetChatMessageString(const FString& InChatMessageString);
	void PrintChatMessageString(const FString& PlayerName, const FString& InChatMessageString, int32 Style);
	void NotificationChatMessageString(const FString& InChatMessageString);
	void PrintResultMessageString(const FString& PlayerName, const FString& InResultString, int32 Style);

	UFUNCTION(Client, Reliable)
	void ClientRPCSetPlayerInfo(const FString& PlayerName, const int32& CurrentCount, const int32& MaxCount);

	UFUNCTION(Client, Reliable)
	void ClientRPCUpdateTime(const FString& CurrentPlayerName, const float& LeftTime);

	UFUNCTION(Client, Reliable)
	void ClientRPCPrintChatMessageString(const FString& PlayerName, const FString& InChatMessageString, int32 Style);

	UFUNCTION(Client, Reliable)
	void ClientRPCNotificationMessageString(const FString& InChatMessageString);

	UFUNCTION(Client, Reliable)
	void ClientRPCResultString(const FString& PlayerName, const FString& InChatMessageString, int32 Style);

	UFUNCTION(Server, Reliable)
	void ServerRPCPrintChatMessageString(const FString& InChatMessageString);

	UFUNCTION(Server, Reliable)
	void ServerRPCSubmitAnswer(const FString& InChatMessageString);
	
};
