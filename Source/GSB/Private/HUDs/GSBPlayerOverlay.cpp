// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBPlayerOverlay.h"
#include "HUDs/GSBWindowWidget.h"
#include "HUDs/GSBInteractionWidget.h"
#include "Components/NamedSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "PlayerController/GSBPlayerController.h"

UGSBWindowWidget* UGSBPlayerOverlay::OpenWindow()
{
	if (UGSBWindowWidget* WindowWidget = Super::OpenWindow())
	{
		if (WindowWidgets.Num() == 1 && IsValid(PlayerController))
		{
			PlayerController->SetUIControlMode(true);
		}
		return WindowWidget;
	}

	return nullptr;
}

void UGSBPlayerOverlay::CloseWindow(UGSBWindowWidget* WindowWidget)
{
	Super::CloseWindow(WindowWidget);

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
