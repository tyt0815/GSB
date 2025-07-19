// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBConstructableFacilitySlot.h"
#include "GSBFacilityQuickSlot.generated.h"

/**
 * 
 */
UCLASS()
class GSB_API UGSBFacilityQuickSlot : public UGSBConstructableFacilitySlot
{
	GENERATED_BODY()
public:
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

public:
	void UpdateFacilityData(UGSBFacilityDataAsset* InFacilityData, int32 InQuickSlotIndex);

	void SetFacilityQuickSlot(UGSBFacilityDataAsset* InFacilityData);

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* QuickSlotIndexText;

	int32 QuickSlotIndex;
};
