// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBWindowHead.h"
#include "GSBFacilityPowerSwitch.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnPowerSwitchClickedSignature, class UGSBFacilityPowerSwitch*, PowerSwitchWidget);


UCLASS()
class GSB_API UGSBFacilityPowerSwitch : public UGSBWindowHead
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

public:
	void TurnOn();

	void TurnOff();

	FOnPowerSwitchClickedSignature OnPowerSwitchClicked;
	
private:
	UFUNCTION()
	void HandleOnPowerSwitchClicked();

	UPROPERTY(meta = (BindWidget))
	class UButton* PowerSwitch;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PowerStateToSwitch;
};
