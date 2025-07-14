// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBContextMenuEntry.h"
#include "HUDs/GSBContextMenu.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UGSBContextMenuEntry::NativeConstruct()
{
	Super::NativeConstruct();

	SetIsFocusable(false);
	LabelButton->SetClickMethod(EButtonClickMethod::MouseDown);
	LabelButton->OnClicked.AddDynamic(this, &UGSBContextMenuEntry::HandleOnLabelButtonClicked);
}

void UGSBContextMenuEntry::SetLabelText(const FText& Text)
{
	LabelText->SetText(Text);
}

void UGSBContextMenuEntry::OnAddedtoContextMenu(UGSBContextMenu* InParentContextMenu, const FText& Text)
{
	SetLabelText(Text);
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
