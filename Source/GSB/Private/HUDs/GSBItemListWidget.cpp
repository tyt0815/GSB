// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBItemListWidget.h"
#include "HUDs/GSBStorageWidget.h"
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
			ItemList->AddChild(ItemSlot);
			ItemSlot->OnAttachedToItemList(this);
			ItemSlot->UpdateItemData(ItemStack);
			return &ItemSlot->OnItemSlotClickedDelegates;
		}
	}

	return nullptr;
}

void UGSBItemListWidget::OnAttachedToStorageWidget(UGSBStorageWidget* ParentStorageWidget)
{
	SetTargetObject(ParentStorageWidget->GetTargetObject());
}
