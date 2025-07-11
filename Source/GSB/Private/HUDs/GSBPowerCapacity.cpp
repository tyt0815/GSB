// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBPowerCapacity.h"
#include "Components/TextBlock.h"

void UGSBPowerCapacity::UpdatePowerCapacity(int32 CurrentPowerUsage, int32 TotalPowerCapacity)
{
	if (CurrentPowerUsage > TotalPowerCapacity)
	{
		SetColorAndOpacity(FColor::Red);
	}
	else
	{
		SetColorAndOpacity(FColor::White);
	}
	PowerCapacityText->SetText(FText::FromString(FString::FromInt(CurrentPowerUsage) + "/" + FString::FromInt(TotalPowerCapacity)));
}
