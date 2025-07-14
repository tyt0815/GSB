// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBWindow.h"
#include "GSBHubDetailWindow.generated.h"

class UGSBStorage;
class UGSBInventory;
class UGSBItemList;
class UGSBItemSlot;

UCLASS()
class GSB_API UGSBHubDetailWindow : public UGSBWindow
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

public:
	void LinkHubStorageComponent(class UItemStorageComponent* HubStorageComponent);

	void LinkInventoryComponent(class UInventoryComponent* InventoryComponent);

private:
	UPROPERTY(meta = (BindWidget))
	UGSBStorage* HubStorage;

	UPROPERTY(meta = (BindWidget))
	UGSBInventory* Inventory;

	UFUNCTION()
	void HandleOnHubStorageItemSlotAdded(UGSBStorage* StorageBody, UGSBItemList* ItemList, UGSBItemSlot* ItemSlot);

	UFUNCTION()
	void HandleOnHubStorageItemSlotLeftClicked(UGSBItemSlot* ItemSlot);

	UFUNCTION()
	void HandleOnInventoryItemSlotAdded(UGSBStorage* StorageBody, UGSBItemList* ItemList, UGSBItemSlot* ItemSlot);

	UFUNCTION()
	void HandleOnInventoryItemSlotLeftClicked(UGSBItemSlot* ItemSlot);
};
