// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBConstructableFacilitySlot.h"
#include "BuildSystem/FacilityBuilder.h"
#include "Characters/GSBPlayer.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Facility/GSBFacilityDataAsset.h"

void UGSBConstructableFacilitySlot::NativeConstruct()
{
	Super::NativeConstruct();

	SlotButton->OnClicked.AddDynamic(this, &UGSBConstructableFacilitySlot::PreviewFacility);
}

void UGSBConstructableFacilitySlot::UpdateFacilityData(UGSBFacilityDataAsset* InFacilityData)
{
	FacilityData = InFacilityData;

	if (IsValid(FacilityData))
	{
		FacilityName->SetText(FacilityData->FacilityName);
	}
	else
	{
		FacilityName->SetText(FText());
	}
}

void UGSBConstructableFacilitySlot::PreviewFacility()
{
	if (IsValid(FacilityData))
	{
		if (AGSBPlayer* Player = GetOwningPlayerPawn<AGSBPlayer>())
		{
			if (AFacilityBuilder* FacilityBuilder = Player->GetFacilityBuilder())
			{
				FacilityBuilder->PreviewFacilityByFacilityData(FacilityData);
			}
		}
	}
}