// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBWidget.h"
#include "GSBPowerCapacity.generated.h"

class UTextBlock;

UCLASS()
class GSB_API UGSBPowerCapacity : public UGSBWidget
{
	GENERATED_BODY()
public:
	void UpdatePowerCapacity(int32 CurrentPowerUsage, int32 TotalPowerCapacity);
	
private	:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PowerCapacityText;
};
