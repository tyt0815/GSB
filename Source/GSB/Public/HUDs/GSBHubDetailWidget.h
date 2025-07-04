// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBFacilityDetailWidget.h"
#include "GSBHubDetailWidget.generated.h"

class UGSBStorageWidget;
class UGSBInventoryWidget;
class UGSBItemSlotWidget;
class UItemStorageComponent;
class UInventoryComponent;
class UItemDataAsset;

UCLASS()
class GSB_API UGSBHubDetailWidget : public UGSBFacilityDetailWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
public:
	void SetHubStorageAndInventory(UItemStorageComponent* HubStorageComponent, UItemStorageComponent* InventoryComponent);

private:
	UPROPERTY(meta = (BindWidget))
	UGSBStorageWidget* HubStorage;

	UPROPERTY(meta = (BindWidget))
	UGSBInventoryWidget* Inventory;

	UFUNCTION()
	void MoveItemFromHubStorageToInventory(UGSBItemSlotWidget* ItemSlotWidget);

	UFUNCTION()
	void MoveItemFromInventoryToHubStorage(UGSBItemSlotWidget* ItemSlotWidget);
};
