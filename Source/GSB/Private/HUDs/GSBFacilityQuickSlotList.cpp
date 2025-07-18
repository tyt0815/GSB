// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBFacilityQuickSlotList.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Characters/GSBPlayer.h"
#include "BuildSystem/FacilityBuilder.h"

void UGSBFacilityQuickSlotList::UpdateFacilityQuickSlots()
{
	ConstructableFacilityQuickSlotList->ClearChildren();
	if (AGSBPlayer* Player = GetOwningPlayerPawn<AGSBPlayer>())
	{
		if (AFacilityBuilder* FacilityBuilder = Player->GetFacilityBuilder())
		{
			AddFacilityQuickSlots(FacilityBuilder->GetFacilityPreviewQuickSlot());
		}
	}
}

void UGSBFacilityQuickSlotList::AddFacilityQuickSlots(const TStaticArray<UGSBFacilityDataAsset*, 10>& FacilityPreviewQuickSlots)
{
	for (int i = 0; i < 10; ++i)
	{
		int32 Index = (i + 1) % 10;
		UGSBFacilityDataAsset* FacilityData = FacilityPreviewQuickSlots[Index];
		if (QuickSlotClass)
		{
			if (UGSBFacilityQuickSlot* QuickSlot = CreateWidget<UGSBFacilityQuickSlot>(GetOwningPlayer(), QuickSlotClass))
			{
				QuickSlot->UpdateFacilityData(FacilityData, Index);
				if (UVerticalBoxSlot* VerticalBoxSlot = Cast<UVerticalBoxSlot>(ConstructableFacilityQuickSlotList->AddChild(QuickSlot)))
				{
					VerticalBoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
				}
			}
		}
	}
}
