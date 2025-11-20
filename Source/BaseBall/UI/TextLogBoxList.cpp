#include "TextLogBoxList.h"

#include "TextLogBox.h"

#include "Blueprint/WidgetTree.h"

UTextLogBox* UTextLogBoxList::CreateLogBox(int32 Style)
{
	if (!LogBoxSytleClasses.IsValidIndex(Style)) return nullptr;
	if (!IsValid(LogBoxSytleClasses[Style])) return nullptr;

	return WidgetTree->ConstructWidget<UTextLogBox>(LogBoxSytleClasses[Style]);
}
