// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GSBFacilityPowerStatus.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFacilityPowerSwitchClickedSignature);

UCLASS()
class GSB_API UGSBFacilityPowerStatus : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

public:
	void SetPowerStatus_Powered();
	
	void SetPowerStatus_Unpowered();

	void SetPowerStatus_TurnedOff();

	void SetPowerConsumption(int32 Value);

	FOnFacilityPowerSwitchClickedSignature OnPowerSwitchClicked;
	
private	:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PowerStatusText;

	UPROPERTY(meta = (BindWidget))
	class UButton* PowerSwitch;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PowerConsumption;

	UFUNCTION()
	void HandleOnPowerSwitchClicked();
};
