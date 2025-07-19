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
	virtual void NativeConstruct() override;

public:
	void SyncronizeFacilityQuickSlots();

	UGSBFacilityQuickSlot* GetFacilityQuickSlotAt(int32 Index);

public:
	void UpdateFacilityQuickSlot(UGSBFacilityDataAsset* FacilityData, int32 Index);

	UPROPERTY(EditDefaultsOnly, Category = "GSB|GSBFacilityQuickSlotList")
	TSubclassOf<UGSBFacilityQuickSlot> QuickSlotClass;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* ConstructableFacilityQuickSlotList;
};
