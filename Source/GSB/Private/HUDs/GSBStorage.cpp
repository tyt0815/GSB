// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBStorage.h"
#include "HUDs/GSBStorageBody.h"

void UGSBStorage::NativeConstruct()
{
	Super::NativeConstruct();

	StorageBody->OnItemSlotAdded.AddDynamic(this, &UGSBStorage::HandleOnItemSlotAdded);
	StorageBody->OnItemSlotContextMenuOpened.AddDynamic(this, &UGSBStorage::HandleOnItemSlotContextMenuOpened);
}

void UGSBStorage::OnClosed()
{
	StorageBody->OnClosed();
	Super::OnClosed();
}

void UGSBStorage::LinkStorageComponent(UItemStorageComponent* StorageComponent)
{
	StorageBody->LinkStorageComponent(StorageComponent);
}

void UGSBStorage::AddItemSlotContextMenuEntry_DropItem()
{
	StorageBody->AddItemSlotContextMenuEntry_DropItem();
}

void UGSBStorage::HandleOnItemSlotAdded(UGSBStorageBody* InStorageBody, UGSBItemList* ItemList, UGSBItemSlot* ItemSlot)
{
	OnItemSlotAdded.Broadcast(this, InStorageBody, ItemList, ItemSlot);
}

void UGSBStorage::HandleOnItemSlotContextMenuOpened(UGSBStorageBody* InStorageBody, UGSBContextMenu* ContextMenu)
{
	OnItemSlotContextMenuOpened.Broadcast(this, InStorageBody, ContextMenu);
}
