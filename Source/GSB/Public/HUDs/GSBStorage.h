// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Items/ItemDataAsset.h"
#include "GSBStorage.generated.h"

class UItemStorageComponent;
class UItemDataAsset;
class UGSBNumberInputDialog;
class UGSBItemList;
class UGSBItemSlot;
class UGSBContextMenu;
class UGSBContextMenuEntry;
class UGSBStorage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnStorageBodyAddItemSlotSignature, UGSBStorage*, StorageBody, UGSBItemList*, ItemList, UGSBItemSlot*, ItemSlot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStorageBodyOpenItemSlotContextMenuSignature, UGSBStorage*, StorageBody, UGSBContextMenu*, ContextMenu);

UCLASS()
class GSB_API UGSBStorage : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

public:
	void LinkStorageComponent(UItemStorageComponent* StorageComponent);

	void UnlinkStorageComponent();

	int32 StoreItem(const FItemStack& ItemStack);

	int32 UnstoreItem(const FItemStack& ItemStack);

	int32 MoveItemTo(UItemStorageComponent* To, FItemStack ItemStack);

	int32 MoveAllItemTo(UItemStorageComponent* To, UItemDataAsset* ItemData);

	FItemStack GetItemStack(UItemDataAsset* ItemData);

	UFUNCTION()
	void DropItemByItemSlotWidget(UGSBItemSlot* ItemSlotWidget);

	UFUNCTION()
	int32 DropItem(const FItemStack& ItemStack);

	int32 DropAllItem(UItemDataAsset* ItemData);

	void AddItemSlotContextMenuEntry_DropItem();

	UFUNCTION()
	void AddItemSlotContextMenuEntry_DropItem_Internal(UGSBStorage* StorageBody, UGSBContextMenu* ContextMenu);

	FOnStorageBodyAddItemSlotSignature OnItemSlotAdded;

	// ItemSlot을 우클릭시 열리는 Context Menu. ContextTarget은 UItemDataAsset* 이다.
	FOnStorageBodyOpenItemSlotContextMenuSignature OnItemSlotContextMenuOpened;

protected:
	void OnLinkedStorageComponent(UItemStorageComponent* StorageComponent);

	void OnUnlinkedStorageComponent();

	void ClearItemList();

	void AddItemSlot(const struct FItemStack& ItemStack);

	UItemStorageComponent* LinkedStorageComponent;

private:
	UPROPERTY(meta = (BindWidget))
	UGSBItemList* ItemList;

	UFUNCTION()
	void HandleOnItemSlotAdded(UGSBItemList* InItemList, UGSBItemSlot* ItemSlot);

	UFUNCTION()
	void HandleOnContextMenuEntryClicked_DropItem(UGSBContextMenuEntry* Entry);

	UFUNCTION()
	void HandleOnNumberInputDialogOKButtonClicked_DropItem(UGSBNumberInputDialog* Dialog);

	UFUNCTION()
	void OpenItemSlotContextMenu(UGSBItemSlot* ItemSlot);


public:
	FORCEINLINE UItemStorageComponent* GetLinkedStorageComponent() const
	{
		return LinkedStorageComponent;
	}

	friend class UItemStorageComponent;
};
