// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ItemStorageComponent.h"
#include "HUDs/GSBStorageBody.h"
#include "Items/DroppedItem.h"
#include "GSBGameInstance.h"

UItemStorageComponent::UItemStorageComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UItemStorageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


void UItemStorageComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

int32 UItemStorageComponent::GetAddableStackCount(const FItemStack& ItemStack) const
{
	if (!IsValid(ItemStack.ItemData))
	{
		return 0;
	}

	int32 Index = FindStoredItemIndex(ItemStack.ItemData);
	if (StoredItems.IsValidIndex(Index))
	{
		return FMath::Clamp(ItemStack.Stack, 0, ItemStack.ItemData->MaxStackCount - StoredItems[Index].Stack);
	}
	else if(StoredItems.Num() < StorageSize)
	{
		return FMath::Clamp(ItemStack.Stack, 0, ItemStack.ItemData->MaxStackCount);
	}

	return 0;
}

FItemStack UItemStorageComponent::GetItemStack(UItemDataAsset* ItemData) const
{
	int32 Index = FindStoredItemIndex(ItemData);
	
	FItemStack ItemStack;
	if (StoredItems.IsValidIndex(Index))
	{
		ItemStack = StoredItems[Index];
	}
	else
	{
		ItemStack.ItemData = ItemData;
		ItemStack.Stack = 0;
	}

	return ItemStack;
}

int32 UItemStorageComponent::FindStoredItemIndex(UItemDataAsset* ItemData) const
{
	if (!IsValid(ItemData))
	{
		return INDEX_NONE;
	}

	for (int i = 0; i < StoredItems.Num(); ++i)
	{
		if (StoredItems[i].ItemData == ItemData)
		{
			return i;
		}
	}

	return INDEX_NONE;
}

int32 UItemStorageComponent::StoreItem(const FItemStack& Item)
{
	if (!IsValid(Item.ItemData) || Item.Stack == 0)
	{
		return 0;
	}

	int32 Index = FindStoredItemIndex(Item.ItemData);

	if (!StoredItems.IsValidIndex(Index))
	{
		if (StoredItems.Num() >= StorageSize)
		{
			return 0;
		}

		FItemStack NewStack;
		NewStack.ItemData = Item.ItemData;
		NewStack.Stack = 0;

		Index = StoredItems.Add(NewStack);
	}

	int32 StackToStore = GetAddableStackCount(Item);	
	
	StoredItems[Index].Stack += StackToStore;

	UpdateStorageWidget();

	return StackToStore;
}

int32 UItemStorageComponent::UnstoreItem(const FItemStack& Item)
{
	if (!IsValid(Item.ItemData))
	{
		return 0;
	}

	int32 Index = FindStoredItemIndex(Item.ItemData);
	if (!StoredItems.IsValidIndex(Index))
	{
		return 0;
	}

	int32 StackToRemove = FMath::Clamp(Item.Stack, 0, StoredItems[Index].Stack);

	StoredItems[Index].Stack -= StackToRemove;

	check(StoredItems[Index].Stack >= 0)
	if (StoredItems[Index].Stack < 1)
	{
		StoredItems.RemoveAt(Index, EAllowShrinking::No);
	}

	UpdateStorageWidget();

	return StackToRemove;
}

int32 UItemStorageComponent::DropItem(FItemStack ItemStack)
{
	UWorld* World = GetWorld();
	UGSBGameInstance* GameInst = World->GetGameInstance<UGSBGameInstance>();
	if (UClass* DroppedItemClass = GameInst->GetActorClass("DroppedItem"))
	{
		ItemStack.Stack = UnstoreItem(ItemStack);
		ADroppedItem* DroppedItem = World->SpawnActor<ADroppedItem>(DroppedItemClass, GetOwner()->GetActorTransform());
		DroppedItem->UpdateItem(ItemStack);
		return ItemStack.Stack;
	}
	return 0;
}

int32 UItemStorageComponent::MoveItemTo(UItemStorageComponent* To, FItemStack ItemStack)
{
	int32 MovedItemNum = 0;
	if (IsValid(To))
	{
		ItemStack.Stack = UnstoreItem(ItemStack);
		MovedItemNum = To->StoreItem(ItemStack);
		ItemStack.Stack -= MovedItemNum;
		StoreItem(ItemStack);
	}
	return MovedItemNum;
}

int32 UItemStorageComponent::MoveAllItemTo(UItemStorageComponent* To, UItemDataAsset* ItemData)
{
	FItemStack ItemStack = GetItemStack(ItemData);
	return MoveItemTo(To, ItemStack);
}

void UItemStorageComponent::LinkStorageWidget(UGSBStorageBody* StorageWidget)
{
	UnlinkStorageWidget();
	LinkedStorageWidget = StorageWidget;
	if (LinkedStorageWidget)
	{
		LinkedStorageWidget->OnLinkedStorageComponent(this);
		UpdateStorageWidget();
	}
}

void UItemStorageComponent::UnlinkStorageWidget()
{
	if (IsValid(LinkedStorageWidget))
	{
		LinkedStorageWidget->OnUnlinkedStorageComponent();
	}
	LinkedStorageWidget = nullptr;
}

void UItemStorageComponent::UpdateStorageWidget()
{
	if (!IsValid(LinkedStorageWidget))
	{
		return;
	}

	LinkedStorageWidget->ClearItemList();

	for (const FItemStack& ItemStack : StoredItems)
	{
		LinkedStorageWidget->AddItemSlot(ItemStack);
	}
}