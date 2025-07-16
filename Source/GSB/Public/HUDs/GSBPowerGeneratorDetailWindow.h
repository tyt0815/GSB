// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBPoweredFacilityDetailWindow.h"
#include "GSBPowerGeneratorDetailWindow.generated.h"

/**
 * 
 */
UCLASS()
class GSB_API UGSBPowerGeneratorDetailWindow : public UGSBPoweredFacilityDetailWindow
{
	GENERATED_BODY()
public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void SetConsumingItem(const FText& ConsumingItemName);

	void SetGeneratingTime(float Seconds);

	void SetConsumingItemSlot(const struct FItemStack& ItemStack);

	void SetRemainingPowerTimeProgressBar(float Progress);

private	:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ConsumingItem;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GeneratingTime;

	UPROPERTY(meta = (BindWidget))
	class UGSBItemSlot* ConsumingItemSlot;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* RemainingPowerTimeProgressBar;
};
