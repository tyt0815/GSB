// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBHubDetailWidget.h"
#include "HUDs/GSBStorageWidget.h"
#include "HUDs/GSBInventoryWidget.h"

void UGSBHubDetailWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HubStorage->SetTitle(FText::FromString(TEXT("허브 저장소")));
	Inventory->SetTitle(FText::FromString(TEXT("인벤토리")));

	HubStorage->OnItemSlotLeftClicked.AddDynamic(this, &UGSBHubDetailWidget::MoveItemFromHubStorageToInventory);
	Inventory->OnItemSlotLeftClicked.AddDynamic(this, &UGSBHubDetailWidget::MoveItemFromInventoryToHubStorage);
}

void UGSBHubDetailWidget::SetHubStorageAndInventory(UItemStorageComponent* HubStorageComponent, UItemStorageComponent* InventoryComponent)
{
	HubStorage->TryLinkStorageComponent(HubStorageComponent);
	Inventory->TryLinkStorageComponent(InventoryComponent);
}

void UGSBHubDetailWidget::MoveItemFromHubStorageToInventory(UGSBItemSlotWidget* ItemSlotWidget)
{
	if (IsValid(HubStorage) && IsValid(ItemSlotWidget))
	{
		HubStorage->MoveAllItemToStorage(Inventory, ItemSlotWidget->GetItemData());
	}
}

void UGSBHubDetailWidget::MoveItemFromInventoryToHubStorage(UGSBItemSlotWidget* ItemSlotWidget)
{
	if (IsValid(Inventory) && IsValid(ItemSlotWidget))
	{
		Inventory->MoveAllItemToStorage(HubStorage, ItemSlotWidget->GetItemData());
	}
}