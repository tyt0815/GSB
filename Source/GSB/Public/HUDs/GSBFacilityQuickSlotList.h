// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDs/GSBFacilityQuickSlot.h"
#include "GSBFacilityQuickSlotList.generated.h"

UCLASS()
class GSB_API UGSBFacilityQuickSlotList : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateFacilityQuickSlots();

private	:
	void AddFacilityQuickSlots(const TStaticArray<UGSBFacilityDataAsset*, 10>& FacilityPreviewQuickSlots);

	UPROPERTY(EditDefaultsOnly, Category = "GSB|GSBFacilityQuickSlotList")
	TSubclassOf<UGSBFacilityQuickSlot> QuickSlotClass;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* ConstructableFacilityQuickSlotList;
};
