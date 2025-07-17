// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBPoweredFacilityDetailWindow.h"
#include "GSBPowerGeneratorDetailWindow.generated.h"

class UGSBStorage;
class UGSBItemList;
class UGSBItemSlot;

UCLASS()
class GSB_API UGSBPowerGeneratorDetailWindow : public UGSBPoweredFacilityDetailWindow
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void OnLinkedToFacility(AFacility* Facility) override;

public:
	void SetConsumingItem(const FText& ConsumingItemName);

	void SetGeneratingTime(float Seconds);

	void SetRemainingPowerTimeProgressBar(float Progress);

private	:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ConsumingItem;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GeneratingTime;

	UPROPERTY(meta = (BindWidget))
	class UGSBInventory* Inventory;

	UPROPERTY(meta = (BindWidget))
	class UGSBStorage* ConsumingItemStorage;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* RemainingPowerTimeProgressBar;

	UFUNCTION()
	void HandleOnInventoryItemSlotAdded(UGSBStorage* StorageBody, UGSBItemList* ItemList, UGSBItemSlot* ItemSlot);

	UFUNCTION()
	void HandleOnInventoryItemSlotLeftClicked(UGSBItemSlot* ItemSlot);

	UFUNCTION()
	void HandleOnConsumingItemStorageItemSlotAdded(UGSBStorage* StorageBody, UGSBItemList* ItemList, UGSBItemSlot* ItemSlot);

	UFUNCTION()
	void HandleOnConsumingItemStorageItemSlotLeftClicked(UGSBItemSlot* ItemSlot);
};
