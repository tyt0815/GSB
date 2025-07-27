// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBContextMenuEntry.h"
#include "HUDs/GSBContextMenu.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UGSBContextMenuEntry::NativeConstruct()
{
	Super::NativeConstruct();

	SetIsFocusable(false);
	EntryButton->SetClickMethod(EButtonClickMethod::MouseDown);
	EntryButton->OnClicked.AddDynamic(this, &UGSBContextMenuEntry::HandleOnLabelButtonClicked);
}

FText UGSBContextMenuEntry::GetEntryDescriptionText()
{
	return EntryDescription->GetText();
}

void UGSBContextMenuEntry::SetEntryDescription(const FText& Text)
{
	EntryDescription->SetText(Text);
}

void UGSBContextMenuEntry::OnAddedtoContextMenu(UGSBContextMenu* InParentContextMenu, const FText& Text)
{
	SetEntryDescription(Text);
	ParentContextMenu = InParentContextMenu;
}

UObject* UGSBContextMenuEntry::GetContextTarget() const
{
	return ParentContextMenu->GetContextTarget();
}

void UGSBContextMenuEntry::CloseContextMenu()
{
	ParentContextMenu->CloseContextMenu();
}

void UGSBContextMenuEntry::HandleOnLabelButtonClicked()
{
	OnClicked.Broadcast(this);
}
