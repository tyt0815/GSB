// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBFacilityPowerSwitch.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UGSBFacilityPowerSwitch::NativeConstruct()
{
	Super::NativeConstruct();

	PowerSwitch->OnClicked.AddDynamic(this, &UGSBFacilityPowerSwitch::HandleOnPowerSwitchClicked);
}

void UGSBFacilityPowerSwitch::TurnOn()
{
	PowerSwitch->SetBackgroundColor(FLinearColor::Red);
	PowerStateToSwitch->SetText(FText::FromString(TEXT("Off")));
}

void UGSBFacilityPowerSwitch::TurnOff()
{
	PowerSwitch->SetBackgroundColor(FLinearColor::Green);
	PowerStateToSwitch->SetText(FText::FromString(TEXT("On")));
}

void UGSBFacilityPowerSwitch::HandleOnPowerSwitchClicked()
{
	OnPowerSwitchClicked.ExecuteIfBound(this);
}
