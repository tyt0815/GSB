// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBFacilityQuickSlotList.h"
#include "Components/HorizontalBox.h"
#include "Characters/GSBPlayer.h"
#include "BuildSystem/FacilityBuilder.h"
#include "DebugHeader.h"

void UGSBFacilityQuickSlotList::NativeConstruct()
{
	Super::NativeConstruct();

	ConstructableFacilityQuickSlotList->ClearChildren();
	for (int i = 0; i < 10; ++i)
	{
		if (QuickSlotClass)
		{
			if (UGSBFacilityQuickSlot* QuickSlot = CreateWidget<UGSBFacilityQuickSlot>(GetOwningPlayer(), QuickSlotClass))
			{
				ConstructableFacilityQuickSlotList->AddChild(QuickSlot);
			}
		}
	}
}

void UGSBFacilityQuickSlotList::SyncronizeFacilityQuickSlots()
{
	if (IsValid(FacilityBuilder))
	{
		FacilityBuilder->SetFacilityQuickSlotListWidget(this);
		const auto& FacilityPreviewQuickSlots = FacilityBuilder->GetFacilityPreviewQuickSlot();
		for (int32 i = 0; i < FacilityPreviewQuickSlots.Num(); ++i)
		{
			UpdateFacilityQuickSlot(FacilityPreviewQuickSlots[i], i);
		}
	}
}

UGSBFacilityQuickSlot* UGSBFacilityQuickSlotList::GetFacilityQuickSlotAt(int32 Index)
{
	if (Index >= 0 && Index < 10)
	{
		return Cast<UGSBFacilityQuickSlot>(ConstructableFacilityQuickSlotList->GetChildAt((Index + 9) % 10));
	}
	return nullptr;
}

void UGSBFacilityQuickSlotList::UpdateFacilityQuickSlot(UGSBFacilityDataAsset* FacilityData, int32 Index)
{
	if (UGSBFacilityQuickSlot* QuickSlot = GetFacilityQuickSlotAt(Index))
	{
		QuickSlot->SetFacilityBuilder(FacilityBuilder);
		QuickSlot->UpdateFacilityData(FacilityData, Index);
	}
}
