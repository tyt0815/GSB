// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBPowerFacilityDetailWindowHead.h"
#include "HUDs/GSBFacilityPowerStatusWidget.h"

void UGSBPowerFacilityDetailWindowHead::SetPowerStatus_Powered()
{
	PowerStatusWidget->SetPowerStatus_Powered();
}

void UGSBPowerFacilityDetailWindowHead::SetPowerStatus_Unpowered()
{
	PowerStatusWidget->SetPowerStatus_Unpowered();
}

void UGSBPowerFacilityDetailWindowHead::SetPowerStatus_TurnedOff()
{
	PowerStatusWidget->SetPowerStatus_TurnedOff();
}
