// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBWindowHead.h"
#include "GSBPowerConsumption.generated.h"

class UTextBlock;

UCLASS()
class GSB_API UGSBPowerConsumption : public UGSBWindowHead
{
	GENERATED_BODY()
public:
	void SetPowerConsumptionText(int32 PowerConsumption);

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PowerConsumptionText;
};
