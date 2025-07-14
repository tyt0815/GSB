// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBHubDetailWindow.h"
#include "HUDs/GSBInventory.h"
#include "HUDs/GSBItemSlot.h"
#include "Components/InventoryComponent.h"


void UGSBHubDetailWindow::NativeConstruct()
{
	Super::NativeConstruct();

	HubStorage->OnItemSlotAdded.AddDynamic(this, &UGSBHubDetailWindow::HandleOnHubStorageItemSlotAdded);
	Inventory->OnItemSlotAdded.AddDynamic(this, &UGSBHubDetailWindow::HandleOnInventoryItemSlotAdded);
}

void UGSBHubDetailWindow::LinkHubStorageComponent(UItemStorageComponent* HubStorageComponent)
{
	HubStorage->LinkStorageComponent(HubStorageComponent);
}

void UGSBHubDetailWindow::LinkInventoryComponent(UInventoryComponent* InventoryComponent)
{
	Inventory->LinkStorageComponent(InventoryComponent);
}

void UGSBHubDetailWindow::HandleOnHubStorageItemSlotAdded(UGSBStorage* StorageBody, UGSBItemList* ItemList, UGSBItemSlot* ItemSlot)
{
	ItemSlot->OnItemSlotLeftClicked.AddDynamic(this, &UGSBHubDetailWindow::HandleOnHubStorageItemSlotLeftClicked);
}

void UGSBHubDetailWindow::HandleOnHubStorageItemSlotLeftClicked(UGSBItemSlot* ItemSlot)
{
	HubStorage->MoveAllItemTo(Inventory->GetLinkedStorageComponent(), ItemSlot->GetItemData());
}

void UGSBHubDetailWindow::HandleOnInventoryItemSlotAdded(UGSBStorage* StorageBody, UGSBItemList* ItemList, UGSBItemSlot* ItemSlot)
{
	ItemSlot->OnItemSlotLeftClicked.AddDynamic(this, &UGSBHubDetailWindow::HandleOnInventoryItemSlotLeftClicked);
}

void UGSBHubDetailWindow::HandleOnInventoryItemSlotLeftClicked(UGSBItemSlot* ItemSlot)
{
	Inventory->MoveAllItemTo(HubStorage->GetLinkedStorageComponent(), ItemSlot->GetItemData());
}
