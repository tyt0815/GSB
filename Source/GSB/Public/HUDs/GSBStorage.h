// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBWindow.h"
#include "GSBStorage.generated.h"

class UItemStorageComponent;
class UItemDataAsset;
class UGSBItemList;
class UGSBItemSlot;
class UGSBContextMenu;
class UGSBContextMenuEntry;
struct FItemStack;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnStorageAddingItemSlotSignature, UGSBStorage*, Storage, UGSBItemList*, ItemList, UGSBItemSlot*, ItemSlot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemSlotContextMenuOpenedSignature, UGSBContextMenu*, ContextMenu);

UCLASS()
class GSB_API UGSBStorage : public UGSBWindow
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	virtual void OnClosed() override;

public:
	void LinkStorageComponent(UItemStorageComponent* StorageComponent);

	void UnlinkStorageComponent();

	UFUNCTION()
	void DropItemByItemSlotWidget(UGSBItemSlot* ItemSlotWidget);

	UFUNCTION()
	int32 DropItem(const FItemStack& ItemStack);

	UFUNCTION() 
	void AddItemSlotContextMenuEntry_DropItem(UGSBContextMenu* ContextMenu);

	FOnStorageAddingItemSlotSignature OnItemSlotAdded;

	// ItemSlot을 우클릭시 열리는 Context Menu. ContextTarget은 UItemDataAsset* 이다.
	FOnItemSlotContextMenuOpenedSignature OnItemSlotContextMenuOpened;

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
	void OpenItemSlotContextMenu(UGSBItemSlot* ItemSlot);

	friend class UItemStorageComponent;
};
