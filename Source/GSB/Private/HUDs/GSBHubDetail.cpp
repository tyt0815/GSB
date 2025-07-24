// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBHubDetail.h"
#include "HUDs/GSBInventory.h"
#include "HUDs/GSBItemSlot.h"
#include "Components/InventoryComponent.h"

void UGSBHubDetail::NativeConstruct()
{
	Super::NativeConstruct();

	HubStorage->OnItemSlotAdded.AddDynamic(this, &UGSBHubDetail::HandleOnHubStorageItemSlotAdded);
	Inventory->OnItemSlotAdded.AddDynamic(this, &UGSBHubDetail::HandleOnInventoryItemSlotAdded);

	HubStorage->AddItemSlotContextMenuEntry_DeleteItem();
	Inventory->AddItemSlotContextMenuEntry_DropItem();
	Inventory->AddItemSlotContextMenuEntry_DeleteItem();
}

void UGSBHubDetail::LinkHubStorageComponent(UItemStorageComponent* HubStorageComponent)
{
	HubStorage->LinkStorageComponent(HubStorageComponent);
}

void UGSBHubDetail::LinkInventoryComponent(UInventoryComponent* InventoryComponent)
{
	Inventory->LinkStorageComponent(InventoryComponent);
}

void UGSBHubDetail::HandleOnHubStorageItemSlotAdded(UGSBStorage* StorageBody, UGSBItemList* ItemList, UGSBItemSlot* ItemSlot)
{
	ItemSlot->OnItemSlotLeftClicked.AddDynamic(this, &UGSBHubDetail::HandleOnHubStorageItemSlotLeftClicked);
}

void UGSBHubDetail::HandleOnHubStorageItemSlotLeftClicked(UGSBItemSlot* ItemSlot)
{
	HubStorage->MoveAllItemTo(Inventory->GetLinkedStorageComponent(), ItemSlot->GetItemData());
}

void UGSBHubDetail::HandleOnInventoryItemSlotAdded(UGSBStorage* StorageBody, UGSBItemList* ItemList, UGSBItemSlot* ItemSlot)
{
	ItemSlot->OnItemSlotLeftClicked.AddDynamic(this, &UGSBHubDetail::HandleOnInventoryItemSlotLeftClicked);
}

void UGSBHubDetail::HandleOnInventoryItemSlotLeftClicked(UGSBItemSlot* ItemSlot)
{
	Inventory->MoveAllItemTo(HubStorage->GetLinkedStorageComponent(), ItemSlot->GetItemData());
}
