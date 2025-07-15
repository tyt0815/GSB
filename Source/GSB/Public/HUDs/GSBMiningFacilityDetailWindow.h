// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBPoweredFacilityDetailWindow.h"
#include "GSBMiningFacilityDetailWindow.generated.h"

/**
 * 
 */
UCLASS()
class GSB_API UGSBMiningFacilityDetailWindow : public UGSBPoweredFacilityDetailWindow
{
	GENERATED_BODY()
public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void OnLinkedToFacility(AFacility* Facility) override;

public:
	void SetMiningItemName(const FName& ItemName);

	void SetMiningTime(float Seconds);

	void SetMiningProgress(float Percent);
	
private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MiningItemName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MiningTime;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* MiningProgress;

	
};
