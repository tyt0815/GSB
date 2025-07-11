// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBItemSlotWidget.h"
#include "HUDs/GSBItemListWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UGSBItemSlotWidget::UpdateItemData(const FItemStack& ItemStack)
{
	if (!IsValid(ItemStack.ItemData))
	{
		ItemNameAndStack->SetText(FText());
		ItemData = nullptr;
		return;
	}

	FString ItemNameAndStackStr = ItemStack.ItemData->Name.ToString();
	if (ItemStack.Stack > 0)
	{
		ItemNameAndStackStr += TEXT("(") + FString::FromInt(ItemStack.Stack) + TEXT(")");
	}
	ItemNameAndStack->SetText(FText::FromString(ItemNameAndStackStr));
	ItemData = ItemStack.ItemData;
}

FReply UGSBItemSlotWidget::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		OnItemSlotClickedDelegates.OnItemSlotLeftClicked.Broadcast(this);
	}
	else if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		OnItemSlotClickedDelegates.OnItemSlotRightClicked.Broadcast(this);
	}
	return FReply::Handled();
}

UGSBItemListWidget* UGSBItemSlotWidget::GetLinkedItemListWidget() const
{
	return Cast<UGSBItemListWidget>(GetTargetObject());
}

void UGSBItemSlotWidget::OnAttachedToItemList(UGSBItemListWidget* ParentItemList)
{
	SetTargetObject(ParentItemList->GetTargetObject());
}
