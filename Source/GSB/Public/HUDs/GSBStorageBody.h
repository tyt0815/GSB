// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GSBStorageBody.generated.h"

class UItemStorageComponent;
class UItemDataAsset;
class UGSBItemList;
class UGSBItemSlot;
class UGSBContextMenu;
class UGSBContextMenuEntry;
struct FItemStack;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnStorageBodyAddItemSlotSignature, UGSBStorageBody*, StorageBody, UGSBItemList*, ItemList, UGSBItemSlot*, ItemSlot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStorageBodyOpenItemSlotContextMenuSignature, UGSBStorageBody*, StorageBody, UGSBContextMenu*, ContextMenu);

UCLASS()
class GSB_API UGSBStorageBody : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

public:
	virtual void OnClosed();

	void LinkStorageComponent(UItemStorageComponent* StorageComponent);

	void UnlinkStorageComponent();

	UFUNCTION()
	void DropItemByItemSlotWidget(UGSBItemSlot* ItemSlotWidget);

	UFUNCTION()
	int32 DropItem(const FItemStack& ItemStack);

	void AddItemSlotContextMenuEntry_DropItem();

	UFUNCTION()
	void AddItemSlotContextMenuEntry_DropItem_Internal(UGSBStorageBody* StorageBody, UGSBContextMenu* ContextMenu);

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
	void OpenItemSlotContextMenu(UGSBItemSlot* ItemSlot);


	friend class UItemStorageComponent;
};
