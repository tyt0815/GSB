// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBContextMenu.h"
#include "HUDs/GSBContextMenuEntry.h"
#include "Components/VerticalBox.h"
#include "Components/CanvasPanelSlot.h"
#include "DebugHeader.h"

void UGSBContextMenu::NativeConstruct()
{
	Super::NativeConstruct();
	SetIsFocusable(true);
	ClearContextMenuEntries();
}

void UGSBContextMenu::NativeOnFocusChanging(const FWeakWidgetPath& PreviousFocusPath, const FWidgetPath& NewWidgetPath, const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusChanging(PreviousFocusPath, NewWidgetPath, InFocusEvent);

	if (!NewWidgetPath.ContainsWidget(&TakeWidget().Get()))
	{
		CloseContextMenu();
	}
}

FGSBContextMenuEntryOnClickedSignature* UGSBContextMenu::AddContextMenuEntry(const FName LabelText)
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (ContextMenuEntryClass)
	{
		if (UGSBContextMenuEntry* Entry = CreateWidget<UGSBContextMenuEntry>(PlayerController, ContextMenuEntryClass))
		{
			EntryList->AddChild(Entry);
			Entry->OnAddedtoContextMenu(this, FText::FromString(LabelText.ToString()));
			return &Entry->ContextMenuEntryOnClicked;
		}
	}
	else
	{
		TRACE_SCREEN_LOG(TEXT("ContextMenuEntryClass가 nullptr입니다."));
	}
	return nullptr;
}

void UGSBContextMenu::ClearContextMenuEntries()
{
	EntryList->ClearChildren();
}

void UGSBContextMenu::CloseContextMenu()
{
	RemoveFromParent();
}
