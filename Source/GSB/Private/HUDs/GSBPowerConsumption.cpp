// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBPowerConsumption.h"
#include "Components/TextBlock.h"

void UGSBPowerConsumption::SetPowerConsumptionText(int32 PowerConsumption)
{
	PowerConsumptionText->SetText(FText::FromString(FString::FromInt(PowerConsumption)));
}
