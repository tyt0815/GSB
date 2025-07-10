// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBWidget.h"
#include "GSBFacilityPowerStatusWidget.generated.h"

class UTextBlock;

UCLASS()
class GSB_API UGSBFacilityPowerStatusWidget : public UGSBWidget
{
	GENERATED_BODY()

public:
	void SetPowerStatus_Powered();
	
	void SetPowerStatus_Unpowered();

	void SetPowerStatus_TurnedOff();
	
private	:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PowerStatusText;
};
