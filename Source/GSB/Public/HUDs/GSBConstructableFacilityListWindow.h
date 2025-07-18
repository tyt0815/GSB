// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBWindow.h"
#include "HUDs/GSBConstructableFacilitySlot.h"
#include "GSBConstructableFacilityListWindow.generated.h"

/**
 * 
 */
UCLASS()
class GSB_API UGSBConstructableFacilityListWindow : public UGSBWindow
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

public:
	void AddConstructableFacilitySlots(const TArray<class UGSBFacilityDataAsset*>& FacilityDatas);

	void AddConstructableFacilitySlot(class UGSBFacilityDataAsset* FacilityData);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "GSB|GSBConstructableFacilityListWindow")
	TSubclassOf<UGSBConstructableFacilitySlot> ConstructableFacilitySlotClass;
	
private:
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ConstructableFacilityList;
};
