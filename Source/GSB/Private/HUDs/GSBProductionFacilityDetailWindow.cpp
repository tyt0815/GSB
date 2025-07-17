// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBProductionFacilityDetailWindow.h"
#include "HUDs/GSBInventory.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Characters/GSBPlayer.h"
#include "Facility/ProductionFacility.h"

void UGSBProductionFacilityDetailWindow::NativeTick(const FGeometry& MyGeometry, float Seconds)
{
	Super::NativeTick(MyGeometry, Seconds);

	if (AProductionFacility* ProductionFacility = Cast<AProductionFacility>(LinkedFacility))
	{
		ProductionTime->SetText(FText::FromString(FString::SanitizeFloat(ProductionFacility->GetProductionTime())));
		ProductionProgress->SetPercent(ProductionFacility->GetProductionProgress());
	}
}

void UGSBProductionFacilityDetailWindow::OnLinkedToFacility(AFacility* Facility)
{
	Super::OnLinkedToFacility(Facility);

	if (AProductionFacility* ProductionFacility = Cast<AProductionFacility>(Facility))
	{
		InputStorage->LinkStorageComponent(ProductionFacility->GetInputStorageComponent());
		OutputStorage->LinkStorageComponent(ProductionFacility->GetOutputStorageComponent());
	}

	if (AGSBPlayer* Player = Cast<AGSBPlayer>(GetOwningPlayerPawn()))
	{
		Inventory->LinkInventoryComponent(Player->GetInventoryComponent());
	}
}