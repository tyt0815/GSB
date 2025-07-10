// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBFacilityPowerStatusWidget.h"
#include "Components/TextBlock.h"

void UGSBFacilityPowerStatusWidget::SetPowerStatus_Powered()
{
	PowerStatusText->SetColorAndOpacity(FColor::Green);
}

void UGSBFacilityPowerStatusWidget::SetPowerStatus_Unpowered()
{
	PowerStatusText->SetColorAndOpacity(FColor::Red);
}

void UGSBFacilityPowerStatusWidget::SetPowerStatus_TurnedOff()
{
	PowerStatusText->SetColorAndOpacity(FColor(0.2));
}
