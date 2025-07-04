// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBItemListWidget.h"
#include "Components/ScrollBox.h"

void UGSBItemListWidget::ClearItemList()
{
	ItemList->ClearChildren();
}

FOnItemSlotClickedDelegates* UGSBItemListWidget::AddItemSlot(const FItemStack& ItemStack)
{
	if (ItemSlotClass)
	{
		if (UGSBItemSlotWidget* ItemSlot = CreateWidget<UGSBItemSlotWidget>(GetOwningPlayer(), ItemSlotClass))
		{
			ItemSlot->UpdateItemData(ItemStack);
			ItemList->AddChild(ItemSlot);
			return &ItemSlot->OnItemSlotClickedDelegates;
		}
	}

	return nullptr;
}