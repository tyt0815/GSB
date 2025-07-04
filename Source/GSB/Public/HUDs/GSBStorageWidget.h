// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBWindowBody.h"
#include "HUDs/GSBItemSlotWidget.h"
#include "HUDs/GSBContextMenuWidget.h"
#include "GSBStorageWidget.generated.h"

class UTextBlock;
class UGSBItemListWidget;
class UItemStorageComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStorageContextWidgetCreatedSignature, UGSBContextMenuWidget*, ContextMenuWidget);

UCLASS()
class GSB_API UGSBStorageWidget : public UGSBWindowBody
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
public:
	virtual void UpdateWidget();

	bool TryLinkStorageComponent(UItemStorageComponent* StorageComponent);

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
	void DefaultHandleOnStorageContextWidgetCreated(UGSBContextMenuWidget* ContextMenuWidget);

	UFUNCTION()
	void AddContextMenuEntry_DeleteItem(UGSBContextMenuWidget* ContextMenuWidget);

protected:
	UFUNCTION()
	void HandleItemSlotLeftClicked(UGSBItemSlotWidget* ItemSlotWidget);

	UFUNCTION()
	void ShowItemSlotContextMenu(UGSBItemSlotWidget* ItemSlotWidget);

	UItemStorageComponent* LinkedItemStorageComponent;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGSBContextMenuWidget> ContextMenuWidgetClass;

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Title;

	UPROPERTY(meta = (BindWidget))
	UGSBItemListWidget* ItemList;

	UFUNCTION()
	void DeleteItem(UWidget* ContextTarget, UGSBContextMenuEntry* MenuEntry);
};
