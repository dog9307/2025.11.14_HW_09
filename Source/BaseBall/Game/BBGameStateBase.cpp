#include "BBGameStateBase.h"

#include "Kismet/GameplayStatics.h"

#include "../Player/BBPlayerController.h"

#include "CommonMacros.h"

void ABBGameStateBase::MulticastRPCBroadcastLoginMessage_Implementation(const FString& InNameString)
{
	if (!HasAuthority())
	{
		VALID_CHECK(APlayerController, PC, UGameplayStatics::GetPlayerController(GetWorld(), 0), );
		CAST_VALID_CHECK(ABBPlayerController, BBPC, Cast<ABBPlayerController>(PC), );

		FString NotificationString = InNameString + TEXT(" has joined the game.");
		BBPC->NotificationChatMessageString(NotificationString);
	}
}