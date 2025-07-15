// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBMiningFacilityDetailWindow.h"
#include "Facility/MiningFacility.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UGSBMiningFacilityDetailWindow::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (AMiningFacility* MiningFacility = Cast<AMiningFacility>(LinkedFacility))
	{
		SetMiningProgress(MiningFacility->GetMiningProgress());
	}
}

void UGSBMiningFacilityDetailWindow::OnLinkedToFacility(AFacility* Facility)
{
	Super::OnLinkedToFacility(Facility);

	if (AMiningFacility* MiningFacility = Cast<AMiningFacility>(LinkedFacility))
	{
		SetMiningItemName(MiningFacility->GetMiningItemName());
		SetMiningTime(MiningFacility->GetMiningTime());
	}
}

void UGSBMiningFacilityDetailWindow::SetMiningItemName(const FName& ItemName)
{
	MiningItemName->SetText(FText::FromString(ItemName.ToString()));
}

void UGSBMiningFacilityDetailWindow::SetMiningTime(float Seconds)
{
	MiningTime->SetText(FText::FromString(FString::SanitizeFloat(Seconds)));
}

void UGSBMiningFacilityDetailWindow::SetMiningProgress(float Percent)
{
	MiningProgress->SetPercent(Percent);
}
