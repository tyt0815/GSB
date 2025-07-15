// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBStorageWindow.h"
#include "HUDs/GSBStorage.h"

void UGSBStorageWindow::NativeConstruct()
{
	Super::NativeConstruct();

	StorageBody->OnItemSlotAdded.AddDynamic(this, &UGSBStorageWindow::HandleOnItemSlotAdded);
	StorageBody->OnItemSlotContextMenuOpened.AddDynamic(this, &UGSBStorageWindow::HandleOnItemSlotContextMenuOpened);
	StorageBody->AddItemSlotContextMenuEntry_DropItem();
	StorageBody->AddItemSlotContextMenuEntry_DeleteItem();
}

void UGSBStorageWindow::LinkStorageComponent(UItemStorageComponent* StorageComponent)
{
	StorageBody->LinkStorageComponent(StorageComponent);
}

void UGSBStorageWindow::HandleOnItemSlotAdded(UGSBStorage* InStorageBody, UGSBItemList* ItemList, UGSBItemSlot* ItemSlot)
{
	OnItemSlotAdded.Broadcast(this, InStorageBody, ItemList, ItemSlot);
}

void UGSBStorageWindow::HandleOnItemSlotContextMenuOpened(UGSBStorage* InStorageBody, UGSBContextMenu* ContextMenu)
{
	OnItemSlotContextMenuOpened.Broadcast(this, InStorageBody, ContextMenu);
}
