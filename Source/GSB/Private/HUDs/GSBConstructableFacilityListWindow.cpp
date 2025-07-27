// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBConstructableFacilityListWindow.h"
#include "HUDs/GSBConstructableFacilitySlot.h"
#include "BuildSystem/FacilityBuilder.h"
#include "Components/ScrollBox.h"
#include "PlayerController/GSBPlayerController.h"
#include "DebugHeader.h"

void UGSBConstructableFacilityListWindow::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGSBConstructableFacilityListWindow::UpdateList()
{
	ConstructableFacilityList->ClearChildren();
	if (IsValid(FacilityBuilder))
	{
		AddConstructableFacilitySlot(FacilityBuilder->GetConveyorBeltForwardData());
		AddConstructableFacilitySlots(FacilityBuilder->GetGeneralFacilityData());
		AddConstructableFacilitySlot(FacilityBuilder->GetMiningFacilityData());
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
			FacilitySlot->SetFacilityBuilder(FacilityBuilder);
			FacilitySlot->UpdateFacilityData(FacilityData);
			FacilitySlot->OnLeftClicked.AddDynamic(this, &UGSBConstructableFacilityListWindow::CloseByFacilitySlot);
		}
	}
	else
	{
		TRACE_SCREEN_LOG(TEXT("ConstructableFacilitySlotClass가 nullptr 입니다."));
	}
}

void UGSBConstructableFacilityListWindow::CloseByFacilitySlot(UGSBConstructableFacilitySlot* FacilitySlot)
{
	Close();
	if (AGSBPlayerController* PC = GetOwningPlayer<AGSBPlayerController>())
	{
		if (PC->IsGSBPlayerControlled())
		{
			PC->SetGamePlayMode_PlayerBuildGameOnly();
		}
		else if (PC->IsTopDownBuildPawnControlled())
		{
			PC->SetGamePlayMode_TopViewBuild();
		}
	}
}
