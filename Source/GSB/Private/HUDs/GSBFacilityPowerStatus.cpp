// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBFacilityPowerStatus.h"
#include "Components/TextBlock.h"

void UGSBFacilityPowerStatus::SetPowerStatus_Powered()
{
	PowerStatusText->SetColorAndOpacity(FColor::Green);
}

void UGSBFacilityPowerStatus::SetPowerStatus_Unpowered()
{
	PowerStatusText->SetColorAndOpacity(FColor::Red);
}

void UGSBFacilityPowerStatus::SetPowerStatus_TurnedOff()
{
	PowerStatusText->SetColorAndOpacity(FLinearColor(0.2, 0.2, 0.2, 1));
}
