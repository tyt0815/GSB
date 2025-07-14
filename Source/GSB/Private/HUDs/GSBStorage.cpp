// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBStorage.h"
#include "HUDs/GSBItemList.h"
#include "HUDs/GSBItemSlot.h"
#include "HUDs/GSBContextMenu.h"
#include "HUDs/GSBContextMenuEntry.h"
#include "Components/ItemStorageComponent.h"
#include "SubSystems/GSBWindowSubsystem.h"
#include "DebugHeader.h"

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

void UGSBStorage::DropItemByItemSlotWidget(UGSBItemSlot* ItemSlotWidget)
{
	FItemStack ItemStack = {};
	ItemStack.ItemData = ItemSlotWidget->GetItemData();
	ItemStack.Stack = 1;
	DropItem(ItemStack);
}

int32 UGSBStorage::DropItem(const FItemStack& ItemStack)
{
	if (IsValid(LinkedStorageComponent))
	{
		return LinkedStorageComponent->DropItem(ItemStack);
	}
	return 0;
}

void UGSBStorage::AddItemSlotContextMenuEntry_DropItem(UGSBContextMenu* ContextMenu)
{
	if (UGSBContextMenuEntry* Entry = ContextMenu->AddContextMenuEntry(TEXT("버리기")))
	{
		UItemDataAsset* ItemData = Cast<UItemDataAsset>(Entry->GetContextTarget());
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
	ItemSlot->OnItemSlotRightClicked.AddDynamic(this, &UGSBStorage::OpenItemSlotContextMenu);
}

void UGSBStorage::OpenItemSlotContextMenu(UGSBItemSlot* ItemSlot)
{
	UGSBWindowSubsystem* WindowManager = GetGameInstance()->GetSubsystem<UGSBWindowSubsystem>();

	if (UGSBContextMenu* ContextMenu = WindowManager->OpenDefaultContextMenu(TEXT("ItemSlotContextMenu"), ItemSlot->GetItemData()))
	{
		OnItemSlotContextMenuOpened.Broadcast(ContextMenu);
	}
}
