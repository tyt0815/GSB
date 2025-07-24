// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBFacilityPowerStatus.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UGSBFacilityPowerStatus::NativeConstruct()
{
	Super::NativeConstruct();

	PowerSwitch->OnClicked.AddDynamic(this, &UGSBFacilityPowerStatus::HandleOnPowerSwitchClicked);
}

void UGSBFacilityPowerStatus::SetPowerStatus_Powered()
{
	if (IsValid(PowerStatusText))
	{
		PowerStatusText->SetColorAndOpacity(FColor::Green);
	}
}

void UGSBFacilityPowerStatus::SetPowerStatus_Unpowered()
{
	if (IsValid(PowerStatusText))
	{
		PowerStatusText->SetColorAndOpacity(FColor::Red);
	}
}

void UGSBFacilityPowerStatus::SetPowerStatus_TurnedOff()
{
	if (IsValid(PowerStatusText))
	{
		PowerStatusText->SetColorAndOpacity(FLinearColor(0.2, 0.2, 0.2, 1));
	}
}

void UGSBFacilityPowerStatus::SetPowerConsumption(int32 Value)
{
	if (IsValid(PowerConsumption))
	{
		PowerConsumption->SetText(FText::FromString(FString::Printf(TEXT("소비 전력: %d"), Value)));
	}
}

void UGSBFacilityPowerStatus::HandleOnPowerSwitchClicked()
{
	OnPowerSwitchClicked.Broadcast();
}
