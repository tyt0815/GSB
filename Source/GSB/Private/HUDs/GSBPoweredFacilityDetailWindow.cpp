// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBPoweredFacilityDetailWindow.h"
#include "HUDs/GSBFacilityPowerStatus.h"

void UGSBPoweredFacilityDetailWindow::NativeConstruct()
{
	Super::NativeConstruct();

	PowerStatus->OnPowerSwitchClicked.AddDynamic(this, &UGSBPoweredFacilityDetailWindow::HandleOnPowerSwitchClicked);
}

void UGSBPoweredFacilityDetailWindow::SetPowerStatus_Powered()
{
	if (IsValid(PowerStatus))
	{
		PowerStatus->SetPowerStatus_Powered();
	}
}

void UGSBPoweredFacilityDetailWindow::SetPowerStatus_TurnedOff()
{
	if (IsValid(PowerStatus))
	{
		PowerStatus->SetPowerStatus_TurnedOff();
	}
}

void UGSBPoweredFacilityDetailWindow::SetPowerStatus_Unpowered()
{
	if (IsValid(PowerStatus))
	{
		PowerStatus->SetPowerStatus_Unpowered();
	}
}

void UGSBPoweredFacilityDetailWindow::SetPowerConsumption(int32 Value)
{
	if(IsValid(PowerStatus))
	{
		PowerStatus->SetPowerConsumption(Value);
	}
}

void UGSBPoweredFacilityDetailWindow::HandleOnPowerSwitchClicked()
{
	OnPowerSwitchClicked.Broadcast();
}
