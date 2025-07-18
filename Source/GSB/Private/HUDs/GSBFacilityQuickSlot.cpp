// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBFacilityQuickSlot.h"
#include "Components/TextBlock.h"

void UGSBFacilityQuickSlot::UpdateFacilityData(UGSBFacilityDataAsset* InFacilityData, int32 InQuickSlotIndex)
{
	Super::UpdateFacilityData(InFacilityData);

	QuickSlotIndex = InQuickSlotIndex;
	QuickSlotIndexText->SetText(FText::FromString(FString::FromInt(QuickSlotIndex)));
}
