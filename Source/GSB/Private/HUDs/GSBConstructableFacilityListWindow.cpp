// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBConstructableFacilityListWindow.h"
#include "HUDs/GSBConstructableFacilitySlot.h"
#include "BuildSystem/FacilityBuilder.h"
#include "Characters/GSBPlayer.h"
#include "Components/ScrollBox.h"
#include "DebugHeader.h"

void UGSBConstructableFacilityListWindow::NativeConstruct()
{
	Super::NativeConstruct();

	ConstructableFacilityList->ClearChildren();
	if (AGSBPlayer* Player = GetOwningPlayerPawn<AGSBPlayer>())
	{
		if (AFacilityBuilder* FacilityBuilder = Player->GetFacilityBuilder())
		{
			AddConstructableFacilitySlot(FacilityBuilder->GetConveyorBeltForwardData());
			AddConstructableFacilitySlots(FacilityBuilder->GetGeneralFacilityData());
			AddConstructableFacilitySlot(FacilityBuilder->GetMiningFacilityData());
		}
	}
}

void UGSBConstructableFacilityListWindow::AddConstructableFacilitySlots(const TArray<class UGSBFacilityDataAsset*>& FacilityDatas)
{
	for (UGSBFacilityDataAsset* Data : FacilityDatas)
	{
		AddConstructableFacilitySlot(Data);
	}
}

void UGSBConstructableFacilityListWindow::AddConstructableFacilitySlot(UGSBFacilityDataAsset* FacilityData)
{
	if (IsValid(ConstructableFacilitySlotClass))
	{
		if (UGSBConstructableFacilitySlot* FacilitySlot = CreateWidget< UGSBConstructableFacilitySlot>(GetOwningPlayer(), ConstructableFacilitySlotClass))
		{
			ConstructableFacilityList->AddChild(FacilitySlot);
			FacilitySlot->UpdateFacilityData(FacilityData);
		}
	}
	else
	{
		TRACE_SCREEN_LOG(TEXT("ConstructableFacilitySlotClass가 nullptr 입니다."));
	}
}
