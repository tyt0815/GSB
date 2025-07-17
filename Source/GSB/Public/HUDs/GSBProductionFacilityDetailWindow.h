// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBPoweredFacilityDetailWindow.h"
#include "GSBProductionFacilityDetailWindow.generated.h"

class UItemStorageComponent;
class UGSBStorage;
class UGSBItemList;
class UGSBItemSlot;

UCLASS()
class GSB_API UGSBProductionFacilityDetailWindow : public UGSBPoweredFacilityDetailWindow
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float Seconds) override;

	virtual void OnLinkedToFacility(AFacility* Facility) override;

	
private:
	UPROPERTY(meta = (BindWidget))
	class UGSBInventory* Inventory;

	UPROPERTY(meta = (BindWidget))
	class UGSBStorage* InputStorage;

	UPROPERTY(meta = (BindWidget))
	class UGSBStorage* OutputStorage;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ProductionProgress;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ProductionTime;

	UFUNCTION()
	void HandleOnInventoryItemSlotAdded(UGSBStorage* StorageBody, UGSBItemList* ItemList, UGSBItemSlot* ItemSlot);

	UFUNCTION()
	void HandleOnInventoryItemSlotLeftClicked(UGSBItemSlot* ItemSlotWidget);

	UFUNCTION()
	void HandleOnInputStorageItemSlotAdded(UGSBStorage* StorageBody, UGSBItemList* ItemList, UGSBItemSlot* ItemSlot);

	UFUNCTION()
	void HandleOnInputStorageItemSlotLeftClicked(UGSBItemSlot* ItemSlotWidget);

	UFUNCTION()
	void HandleOnOutputStorageItemSlotAdded(UGSBStorage* StorageBody, UGSBItemList* ItemList, UGSBItemSlot* ItemSlot);

	UFUNCTION()
	void HandleOnOutputStorageItemSlotLeftClicked(UGSBItemSlot* ItemSlotWidget);
};
