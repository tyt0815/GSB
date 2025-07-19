// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBConstructableFacilitySlot.h"
#include "HUDs/GSBFacilitySlotDragDropOperation.h"
#include "BuildSystem/FacilityBuilder.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Characters/GSBPlayer.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Facility/GSBFacilityDataAsset.h"
#include "DebugHeader.h"

FReply UGSBConstructableFacilitySlot::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
	return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
}

FReply UGSBConstructableFacilitySlot::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		PreviewFacility();
	}

	return FReply::Handled();
}

void UGSBConstructableFacilitySlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	FVector2D DragOffset = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
	UGSBFacilitySlotDragDropOperation* DragDropOperation = NewObject<UGSBFacilitySlotDragDropOperation>(this);
	DragDropOperation->FacilityData = FacilityData;
	DragDropOperation->Pivot = EDragPivot::MouseDown;
	UTextBlock* DragVisual = WidgetTree->ConstructWidget<UTextBlock>();
	DragVisual->SetText(FacilityName->GetText());
	DragVisual->SetFont(FacilityName->GetFont());
	DragDropOperation->DefaultDragVisual = DragVisual;

	OutOperation = DragDropOperation;
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