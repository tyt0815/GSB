// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GSBHubDetail.generated.h"

class UGSBStorage;
class UGSBInventory;

UCLASS()
class GSB_API UGSBHubDetail : public UUserWidget
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
