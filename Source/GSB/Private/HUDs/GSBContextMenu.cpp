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
	
}

void UGSBContextMenu::NativeOnFocusChanging(const FWeakWidgetPath& PreviousFocusPath, const FWidgetPath& NewWidgetPath, const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusChanging(PreviousFocusPath, NewWidgetPath, InFocusEvent);

	if (!NewWidgetPath.ContainsWidget(&TakeWidget().Get()))
	{
		CloseContextMenu();
	}
}

void UGSBContextMenu::OnOpened(UObject* InContextTarget)
{
	ClearContextMenuEntries();
	ContextTarget = InContextTarget;
	SetFocus();
}

UGSBContextMenuEntry* UGSBContextMenu::AddContextMenuEntry(const FString& Label)
{
	if (ContextMenuEntryClass)
	{
		UGSBContextMenuEntry* Entry = CreateWidget<UGSBContextMenuEntry>(GetOwningPlayer(), ContextMenuEntryClass, FName(FString::Printf(TEXT("%s_Entry_%s"), *GetName(), *Label)));
		EntryList->AddChild(Entry);
		Entry->OnAddedtoContextMenu(this, FText::FromString(Label));
		return Entry;
	}
	else
	{
		TRACE_SCREEN_LOG(TEXT("ContextMenuEntryClass가 nullptr 입니다."));
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
