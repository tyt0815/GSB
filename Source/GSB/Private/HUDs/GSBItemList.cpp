// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBItemList.h"
#include "Components/ScrollBox.h"
#include "DebugHeader.h"

void UGSBItemList::ClearItemList()
{
	ItemList->ClearChildren();
}

void UGSBItemList::AddItemSlot(const FItemStack& ItemStack)
{
#ifdef WITH_EDITOR
	if (!ItemSlotClass)
	{
		TRACE_SCREEN_LOG(TEXT("ItemSlotClass가 nullptr입니다."));
		return;
	}
#endif

	UGSBItemSlot* ItemSlot = CreateWidget<UGSBItemSlot>(GetOwningPlayer(), ItemSlotClass, FName(GetName() + TEXT("_ItemSlot")));
	ItemList->AddChild(ItemSlot);
	ItemSlot->SetLabelByItemStack(ItemStack);
	OnItemSlotAdded.Broadcast(this, ItemSlot);
}
