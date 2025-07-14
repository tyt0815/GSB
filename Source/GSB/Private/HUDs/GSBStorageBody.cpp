// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBStorageBody.h"
#include "HUDs/GSBItemList.h"
#include "HUDs/GSBItemSlot.h"
#include "HUDs/GSBContextMenu.h"
#include "HUDs/GSBContextMenuEntry.h"
#include "Components/ItemStorageComponent.h"
#include "SubSystems/GSBWindowSubsystem.h"
#include "DebugHeader.h"

void UGSBStorageBody::NativeConstruct()
{
	Super::NativeConstruct();

	ItemList->OnItemSlotAdded.AddDynamic(this, &UGSBStorageBody::HandleOnItemSlotAdded);
}

void UGSBStorageBody::OnClosed()
{
	UnlinkStorageComponent();

	
}

void UGSBStorageBody::LinkStorageComponent(UItemStorageComponent* StorageComponent)
{
	UnlinkStorageComponent();
	if (IsValid(StorageComponent))
	{
		StorageComponent->LinkStorageWidget(this);
	}
}

void UGSBStorageBody::UnlinkStorageComponent()
{
	if (IsValid(LinkedStorageComponent))
	{
		LinkedStorageComponent->UnlinkStorageWidget();
	}
}

void UGSBStorageBody::DropItemByItemSlotWidget(UGSBItemSlot* ItemSlotWidget)
{
	FItemStack ItemStack = {};
	ItemStack.ItemData = ItemSlotWidget->GetItemData();
	ItemStack.Stack = 1;
	DropItem(ItemStack);
}

int32 UGSBStorageBody::DropItem(const FItemStack& ItemStack)
{
	if (IsValid(LinkedStorageComponent))
	{
		return LinkedStorageComponent->DropItem(ItemStack);
	}
	return 0;
}

void UGSBStorageBody::AddItemSlotContextMenuEntry_DropItem()
{
	OnItemSlotContextMenuOpened.AddDynamic(this, &UGSBStorageBody::AddItemSlotContextMenuEntry_DropItem_Internal);
}

void UGSBStorageBody::AddItemSlotContextMenuEntry_DropItem_Internal(UGSBStorageBody* StorageBody, UGSBContextMenu* ContextMenu)
{
	if (UGSBContextMenuEntry* Entry = ContextMenu->AddContextMenuEntry(TEXT("버리기")))
	{
		Entry->OnClicked.AddDynamic(this, &UGSBStorageBody::HandleOnContextMenuEntryClicked_DropItem);
		UItemDataAsset* ItemData = Cast<UItemDataAsset>(Entry->GetContextTarget());
	}
}

void UGSBStorageBody::OnLinkedStorageComponent(UItemStorageComponent* StorageComponent)
{
	LinkedStorageComponent = StorageComponent;
}

void UGSBStorageBody::OnUnlinkedStorageComponent()
{
	LinkedStorageComponent = nullptr;
}

void UGSBStorageBody::ClearItemList()
{
	ItemList->ClearItemList();
}

void UGSBStorageBody::AddItemSlot(const FItemStack& ItemStack)
{
	ItemList->AddItemSlot(ItemStack);
}

void UGSBStorageBody::HandleOnItemSlotAdded(UGSBItemList* InItemList, UGSBItemSlot* ItemSlot)
{
	OnItemSlotAdded.Broadcast(this, InItemList, ItemSlot);
	ItemSlot->OnItemSlotRightClicked.AddDynamic(this, &UGSBStorageBody::OpenItemSlotContextMenu);
}

void UGSBStorageBody::HandleOnContextMenuEntryClicked_DropItem(UGSBContextMenuEntry* Entry)
{
	if (UItemDataAsset* ItemData = Cast<UItemDataAsset>(Entry->GetContextTarget()))
	{
		DropItem(FItemStack(ItemData, 1));
	}
}

void UGSBStorageBody::OpenItemSlotContextMenu(UGSBItemSlot* ItemSlot)
{
	UGSBWindowSubsystem* WindowManager = GetGameInstance()->GetSubsystem<UGSBWindowSubsystem>();

	if (UGSBContextMenu* ContextMenu = WindowManager->OpenDefaultContextMenu(TEXT("ItemSlotContextMenu"), ItemSlot->GetItemData()))
	{
		OnItemSlotContextMenuOpened.Broadcast(this, ContextMenu);
	}
}