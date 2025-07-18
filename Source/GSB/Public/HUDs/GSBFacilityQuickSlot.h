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
	void UpdateFacilityData(UGSBFacilityDataAsset* InFacilityData, int32 InQuickSlotIndex);

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* QuickSlotIndexText;

	int32 QuickSlotIndex;
};
