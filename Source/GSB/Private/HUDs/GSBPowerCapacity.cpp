// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBPowerCapacity.h"
#include "Components/TextBlock.h"

void UGSBPowerCapacity::UpdateWidget(int32 NewPowerUsage, int32 NewPowerCapacity)
{
	PowerUsage = NewPowerUsage;
	PowerCapacity = NewPowerCapacity;
	if (NewPowerUsage > NewPowerCapacity)
	{
		SetColorAndOpacity(FColor::Red);
	}
	else
	{
		SetColorAndOpacity(FColor::White);
	}
	PowerCapacityText->SetText(FText::FromString(FString::FromInt(NewPowerUsage) + "/" + FString::FromInt(NewPowerCapacity)));
}

void UGSBPowerCapacity::UpdatePowerUsage(int32 NewPowerUsage)
{
	UpdateWidget(NewPowerUsage, PowerCapacity);
}

void UGSBPowerCapacity::UpdatePowerCapacity(int32 NewPowerCapacity)
{
	UpdateWidget(PowerUsage, NewPowerCapacity);
}
