// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBFacilityQuickSlot.h"
#include "HUDs/GSBFacilitySlotDragDropOperation.h"
#include "Components/TextBlock.h"
#include "Characters/GSBPlayer.h"
#include "BuildSystem/FacilityBuilder.h"
#include "DebugHeader.h"

void UGSBFacilityQuickSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	SetFacilityQuickSlot(nullptr);
}

bool UGSBFacilityQuickSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (UGSBFacilitySlotDragDropOperation* DragDrop = Cast<UGSBFacilitySlotDragDropOperation>(InOperation))
	{
		SetFacilityQuickSlot(DragDrop->FacilityData);
		return true;
	}
	return false;
}

void UGSBFacilityQuickSlot::UpdateFacilityData(UGSBFacilityDataAsset* InFacilityData, int32 InQuickSlotIndex)
{
	Super::UpdateFacilityData(InFacilityData);

	QuickSlotIndex = InQuickSlotIndex;
	QuickSlotIndexText->SetText(FText::FromString(FString::FromInt(QuickSlotIndex)));
}

void UGSBFacilityQuickSlot::SetFacilityQuickSlot(UGSBFacilityDataAsset* InFacilityData)
{
	if (AGSBPlayer* Player = GetOwningPlayerPawn<AGSBPlayer>())
	{
		if (AFacilityBuilder* FacilityBuilder = Player->GetFacilityBuilder())
		{
			FacilityBuilder->SetFacilityQuickSlot(InFacilityData, QuickSlotIndex);
		}
	}
}