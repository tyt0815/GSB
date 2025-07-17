// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBPoweredFacilityDetailWindow.h"
#include "GSBProductionFacilityDetailWindow.generated.h"

class UItemStorageComponent;

UCLASS()
class GSB_API UGSBProductionFacilityDetailWindow : public UGSBPoweredFacilityDetailWindow
{
	GENERATED_BODY()
public:
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
};
