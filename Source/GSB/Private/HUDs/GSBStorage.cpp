// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBStorage.h"
#include "HUDs/GSBItemList.h"
#include "Components/ItemStorageComponent.h"

void UGSBStorage::NativeConstruct()
{
	Super::NativeConstruct();

	ItemList->OnItemSlotAdded.AddDynamic(this, &UGSBStorage::HandleOnItemSlotAdded);
}

void UGSBStorage::OnClosed()
{
	UnlinkStorageComponent();

	Super::OnClosed();
}

void UGSBStorage::LinkStorageComponent(UItemStorageComponent* StorageComponent)
{
	UnlinkStorageComponent();
	if (IsValid(StorageComponent))
	{
		StorageComponent->LinkStorageWidget(this);
	}
}

void UGSBStorage::UnlinkStorageComponent()
{
	if (IsValid(LinkedStorageComponent))
	{
		LinkedStorageComponent->UnlinkStorageWidget();
	}
}

void UGSBStorage::OnLinkedStorageComponent(UItemStorageComponent* StorageComponent)
{
	LinkedStorageComponent = StorageComponent;
}

void UGSBStorage::OnUnlinkedStorageComponent()
{
	LinkedStorageComponent = nullptr;
}

void UGSBStorage::ClearItemList()
{
	ItemList->ClearItemList();
}

void UGSBStorage::AddItemSlot(const FItemStack& ItemStack)
{
	ItemList->AddItemSlot(ItemStack);
}

void UGSBStorage::HandleOnItemSlotAdded(UGSBItemList* InItemList, UGSBItemSlot* ItemSlot)
{
	OnItemSlotAdded.Broadcast(this, InItemList, ItemSlot);
}