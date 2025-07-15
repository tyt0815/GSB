// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBStorage.h"
#include "HUDs/GSBNumberInputDialog.h"
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

void UGSBStorage::NativeDestruct()
{
	UnlinkStorageComponent();
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

int32 UGSBStorage::StoreItem(const FItemStack& ItemStack)
{
	if (IsValid(LinkedStorageComponent))
	{
		return LinkedStorageComponent->StoreItem(ItemStack);
	}
	return 0;
}

int32 UGSBStorage::UnstoreItem(const FItemStack& ItemStack)
{
	if (IsValid(LinkedStorageComponent))
	{
		return LinkedStorageComponent->UnstoreItem(ItemStack);
	}
	return 0;
}

int32 UGSBStorage::MoveItemTo(UItemStorageComponent* To, FItemStack ItemStack)
{
	return LinkedStorageComponent->MoveItemTo(To, ItemStack);
}

int32 UGSBStorage::MoveAllItemTo(UItemStorageComponent* To, UItemDataAsset* ItemData)
{
	return LinkedStorageComponent->MoveAllItemTo(To,ItemData);
}

FItemStack UGSBStorage::GetItemStack(UItemDataAsset* ItemData)
{
	return LinkedStorageComponent->GetItemStack(ItemData);
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

int32 UGSBStorage::DropAllItem(UItemDataAsset* ItemData)
{
	return DropItem(LinkedStorageComponent->GetItemStack(ItemData));
}

void UGSBStorage::AddItemSlotContextMenuEntry_DropItem()
{
	OnItemSlotContextMenuOpened.AddDynamic(this, &UGSBStorage::AddItemSlotContextMenuEntry_DropItem_Internal);
}

void UGSBStorage::AddItemSlotContextMenuEntry_DropItem_Internal(UGSBStorage* StorageBody, UGSBContextMenu* ContextMenu)
{
	if (UGSBContextMenuEntry* Entry = ContextMenu->AddContextMenuEntry(TEXT("버리기")))
	{
		Entry->OnClicked.AddDynamic(this, &UGSBStorage::HandleOnContextMenuEntryClicked_DropItem);
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

void UGSBStorage::HandleOnContextMenuEntryClicked_DropItem(UGSBContextMenuEntry* Entry)
{
	if (UItemDataAsset* ItemData = Cast<UItemDataAsset>(Entry->GetContextTarget()))
	{
		UGSBWindowSubsystem* WindowManager = GetGameInstance()->GetSubsystem<UGSBWindowSubsystem>();
		if (UGSBNumberInputDialog* Dialog = WindowManager->OpenDefaultNumberInputDialog(TEXT("DropItemDialog"), ItemData))
		{
			Dialog->SetNumberInput(GetItemStack(ItemData).Stack);
			Dialog->OnOKButtonClicked.AddDynamic(this, &UGSBStorage::HandleOnNumberInputDialogOKButtonClicked_DropItem);
		}
	}
	Entry->CloseContextMenu();
}

void UGSBStorage::HandleOnNumberInputDialogOKButtonClicked_DropItem(UGSBNumberInputDialog* Dialog)
{
	if (UItemDataAsset* ItemData = Cast<UItemDataAsset>(Dialog->GetTargetObject()))
	{
		DropItem({ ItemData, Dialog->GetNumber() });
	}
}

void UGSBStorage::OpenItemSlotContextMenu(UGSBItemSlot* ItemSlot)
{
	UGSBWindowSubsystem* WindowManager = GetGameInstance()->GetSubsystem<UGSBWindowSubsystem>();

	if (UGSBContextMenu* ContextMenu = WindowManager->OpenDefaultContextMenu(TEXT("ItemSlotContextMenu"), ItemSlot->GetItemData()))
	{
		OnItemSlotContextMenuOpened.Broadcast(this, ContextMenu);
	}
}