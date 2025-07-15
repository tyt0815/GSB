// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBFacilityDetailWindow.h"
#include "GSBPoweredFacilityDetailWindow.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDetailWindowPowerSwitchClickedSignature);

UCLASS()
class GSB_API UGSBPoweredFacilityDetailWindow : public UGSBFacilityDetailWindow
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

public:
	void SetPowerStatus_Powered();

	void SetPowerStatus_TurnedOff();

	void SetPowerStatus_Unpowered();

	void SetPowerConsumption(int32 Value);

	FOnDetailWindowPowerSwitchClickedSignature OnPowerSwitchClicked;
	
private:
	UPROPERTY(meta = (BindWidget))
	class UGSBFacilityPowerStatus* PowerStatus;

	UFUNCTION()
	void HandleOnPowerSwitchClicked();
};
