#include "NumButton.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"

void UNumButton::SetNumText()
{
	if (!IsValid(NumText)) return;

	NumText->SetText(FText::FromString(FString::FromInt( CurrentNum)));
}

void UNumButton::NativeConstruct()
{
	Super::NativeConstruct();

	if (!NumButton->OnClicked.IsAlreadyBound(this, &ThisClass::IncreaseNum))
	{
		NumButton->OnClicked.AddDynamic(this, &ThisClass::IncreaseNum);

		ResetNum();
	}
}

void UNumButton::NativeDestruct()
{
	Super::NativeDestruct();

	if (!NumButton->OnClicked.IsAlreadyBound(this, &ThisClass::IncreaseNum))
	{
		NumButton->OnClicked.RemoveDynamic(this, &ThisClass::IncreaseNum);
	}
}

void UNumButton::IncreaseNum()
{
	++CurrentNum;
	CurrentNum = (CurrentNum > MAX ? MIN : CurrentNum);
	//CurrentNum = (CurrentNum - MIN + 1) % (MAX - MIN + 1) + MIN;

	SetNumText();
}

void UNumButton::DecreaseNum()
{
	--CurrentNum;
	CurrentNum = (CurrentNum < MIN ? MAX : CurrentNum);

	SetNumText();
}

void UNumButton::ResetNum()
{
	CurrentNum = MIN;

	SetNumText();
}

int32 UNumButton::GetNum()
{
	return CurrentNum;
}
