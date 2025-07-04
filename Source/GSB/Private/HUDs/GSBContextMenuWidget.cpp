// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBContextMenuWidget.h"
#include "HUDs/GSBContextMenuEntry.h"
#include "Components/VerticalBox.h"

void UGSBContextMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetIsFocusable(true);
}

void UGSBContextMenuWidget::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusLost(InFocusEvent);

	// RemoveFromParent();
}

void UGSBContextMenuWidget::NativeOnFocusChanging(const FWeakWidgetPath& PreviousFocusPath, const FWidgetPath& NewWidgetPath, const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusChanging(PreviousFocusPath, NewWidgetPath, InFocusEvent);

	if (!NewWidgetPath.ContainsWidget(&TakeWidget().Get()))
	{
		RemoveFromParent();
	}
}

FGSBContextMenuEntryOnClickedSignature* UGSBContextMenuWidget::AddContextMenuEntry(const FName LabelText)
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (ContextMenuEntryClass && IsValid(PlayerController))
	{
		if (UGSBContextMenuEntry* Entry = CreateWidget<UGSBContextMenuEntry>(PlayerController, ContextMenuEntryClass))
		{
			Entry->SetLabelText(FText::FromString(LabelText.ToString()));
			EntryList->AddChild(Entry);
			return &Entry->ContextMenuEntryOnClicked;
		}
	}
	return nullptr;
}

void UGSBContextMenuWidget::SetContextTarget(UWidget* ContextTarget)
{
	for (UWidget* Widget : EntryList->GetAllChildren())
	{
		if (UGSBContextMenuEntry* Entry = Cast<UGSBContextMenuEntry>(Widget))
		{
			Entry->SetContextTarget(ContextTarget);
		}
	}
}

void UGSBContextMenuWidget::ClearContextMenuEntries()
{
	EntryList->ClearChildren();
}
