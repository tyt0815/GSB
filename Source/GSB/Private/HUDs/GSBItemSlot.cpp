// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBItemSlot.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Items/ItemDataAsset.h"

FReply UGSBItemSlot::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		OnItemSlotLeftClicked.Broadcast(this);
	}
	else if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		OnItemSlotRightClicked.Broadcast(this);
	}
	return FReply::Handled();
}

void UGSBItemSlot::SetLabelByItemDataAsset(UItemDataAsset* InItemData)
{
	if (IsValid(InItemData))
	{
		Label->SetText(InItemData->Name);
	}
	else
	{
		Label->SetText(FText());
	}

	ItemData = InItemData;
}

void UGSBItemSlot::SetLabelByItemStack(const FItemStack& ItemStack)
{
	if (IsValid(ItemStack.ItemData))
	{
		FString LabelString = ItemStack.ItemData->Name.ToString() + FString::Printf(TEXT(" (%d)"), ItemStack.Stack);
		Label->SetText(FText::FromString(LabelString));
	}
	else
	{
		Label->SetText(FText());
	}

	ItemData = ItemStack.ItemData;
}
