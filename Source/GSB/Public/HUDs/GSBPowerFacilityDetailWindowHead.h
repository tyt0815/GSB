// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBWindowHead.h"
#include "GSBPowerFacilityDetailWindowHead.generated.h"



UCLASS()
class GSB_API UGSBPowerFacilityDetailWindowHead : public UGSBWindowHead
{
	GENERATED_BODY()
public:
	void SetPowerStatus_Powered();

	void SetPowerStatus_Unpowered();

	void SetPowerStatus_TurnedOff();
	
private:
	UPROPERTY(meta = (BindWidget))
	class UGSBFacilityPowerStatusWidget* PowerStatusWidget;
};
