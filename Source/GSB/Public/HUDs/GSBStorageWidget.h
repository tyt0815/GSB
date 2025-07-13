// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBWindowBody.h"
#include "HUDs/GSBItemSlotWidget.h"
#include "HUDs/GSBContextMenu.h"
#include "GSBStorageWidget.generated.h"

class UTextBlock;
class UGSBItemListWidget;
class UGSBContextMenu;
class UGSBConfirmationDialog;
class UItemStorageComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStorageContextWidgetCreatedSignature, UGSBContextMenu*, ContextMenuWidget);

UCLASS()
class GSB_API UGSBStorageWidget : public UGSBWindowBody
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
public:
	virtual void UpdateWidget();

	void OnLinkedStorageComponent(UItemStorageComponent* StorageComponent);

	void SetTitle(const FText& NewTitle);

	FItemStack GetItemStack(UItemDataAsset* ItemData) const;

	int32 StoreItemStack(const FItemStack& ItemStack);

	int32 UnStoreItemStack(const FItemStack& ItemStack);

	int32 MoveItemToStorage(UGSBStorageWidget* To, const FItemStack& ItemStack);

	int32 MoveAllItemToStorage(UGSBStorageWidget* To, UItemDataAsset* ItemData);

	FOnItemSlotLeftClicked OnItemSlotLeftClicked;

	FOnStorageContextWidgetCreatedSignature OnStorageContextWidgetCreated;

	UFUNCTION()
	void AddDefaultContextMenuEntry(UGSBContextMenu* ContextMenuWidget);

protected:
	UFUNCTION()
	void HandleItemSlotLeftClicked(UGSBItemSlotWidget* ItemSlotWidget);

	UFUNCTION()
	void HandleOnConfirmDeleteButtonClicked(UGSBConfirmationDialog* ConfirmationDialog);

	UFUNCTION()
	void OpenItemSlotContextMenu(UGSBItemSlotWidget* ItemSlotWidget);

	UItemStorageComponent* LinkedItemStorageComponent;

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Title;

	UPROPERTY(meta = (BindWidget))
	UGSBItemListWidget* ItemList;

	UFUNCTION()
	void HandleContextMenuEntry_DeleteItem(UGSBContextMenuEntry* MenuEntry);
};
