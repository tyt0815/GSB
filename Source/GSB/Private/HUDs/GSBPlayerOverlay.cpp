// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBPlayerOverlay.h"
#include "HUDs/GSBWindowWidget.h"
#include "HUDs/GSBInteractionList.h"
#include "HUDs/GSBPowerCapacity.h"
#include "Components/NamedSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "PlayerController/GSBPlayerController.h"

UGSBWindowWidget* UGSBPlayerOverlay::OpenWindow_Internal(UObject* InTargetObject)
{
	if (UGSBWindowWidget* WindowWidget = Super::OpenWindow_Internal(InTargetObject))
	{
		if (WindowWidgets.Num() == 1 && IsValid(PlayerController))
		{
			PlayerController->SetUIControlMode(true);
		}
		return WindowWidget;
	}

	return nullptr;
}

void UGSBPlayerOverlay::CloseWindow_Internal(UGSBWindowWidget* WindowWidget)
{
	Super::CloseWindow_Internal(WindowWidget);

	if (WindowWidgets.Num() == 0)
	{
		PlayerController->SetUIControlMode(false);
	}
}

void UGSBPlayerOverlay::InitializeOverlay()
{
	PlayerController = GetOwningPlayer<AGSBPlayerController>();

	HideInteractionList();
}


void UGSBPlayerOverlay::ShowInteractionList()
{
	InteractionListWidget->SetVisibility(ESlateVisibility::Visible);
}

void UGSBPlayerOverlay::HideInteractionList()
{
	InteractionListWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UGSBPlayerOverlay::UpdateInteractionFocusing(int32 Index)
{
	InteractionListWidget->UpdateFocusing(Index);
}

void UGSBPlayerOverlay::UpdateInteractionList(const TArray<FString>& Descriptions)
{
	InteractionListWidget->UpdateInteractionList(Descriptions);
}

void UGSBPlayerOverlay::UpdatePowerCapacity(int32 CurrentPowerUsage, int32 TotalPowerCapacity)
{
	PowerCapacity->UpdatePowerCapacity(CurrentPowerUsage, TotalPowerCapacity);
}