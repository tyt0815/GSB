// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"
#include "HUDs/GSBInventoryWidget.h"

int32 UInventoryComponent::GetAddableStackCount(const FItemStack& ItemStack) const
{
	if (!IsValid(ItemStack.ItemData))
	{
		return 0;
	}

	FItemStack WeightLimitedStack = ItemStack;

	if (ItemStack.ItemData->Weight > 0)
	{
		int32 AvailableWeight = MaxWeight - CurrentWeight;
		int32 FinalWeight = FMath::Clamp(CalcItemStackWeight(ItemStack), 0, AvailableWeight);

		WeightLimitedStack.Stack = FinalWeight / ItemStack.ItemData->Weight;
	}
	
	return Super::GetAddableStackCount(WeightLimitedStack);
}

int32 UInventoryComponent::StoreItem(const FItemStack& Item)
{
	int32 StoredItemNum = Super::StoreItem(Item);
	if (IsValid(Item.ItemData))
	{
		CurrentWeight += Item.ItemData->Weight * StoredItemNum;
		if (UGSBInventoryWidget* InventoryWidget = Cast<UGSBInventoryWidget>(LinkedStorageWidget))
		{
			InventoryWidget->UpdateWeightUI();
		}
	}
	return StoredItemNum;
}

int32 UInventoryComponent::UnstoreItem(const FItemStack& Item)
{
	int32 UnstoredItemNum = Super::UnstoreItem(Item);
	if (IsValid(Item.ItemData))
	{
		CurrentWeight -= Item.ItemData->Weight * UnstoredItemNum;
		if (UGSBInventoryWidget* InventoryWidget = Cast<UGSBInventoryWidget>(LinkedStorageWidget))
		{
			InventoryWidget->UpdateWeightUI();
		}
	}
	return UnstoredItemNum;
}