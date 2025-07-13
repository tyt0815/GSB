// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBPlayerOverlay.h"
#include "HUDs/GSBInteractionList.h"
#include "HUDs/GSBPowerCapacity.h"
#include "PlayerController/GSBPlayerController.h"

void UGSBPlayerOverlay::InitializeOverlay()
{
	PlayerController = GetOwningPlayer<AGSBPlayerController>();

	HideInteractionList();
}

UGSBWindow* UGSBPlayerOverlay::OpenWindow(TSubclassOf<UGSBWindow> WindowClass, const FName& WindowName)
{
	if (UGSBWindow* Window = Super::OpenWindow(WindowClass, WindowName))
	{
		if (OpenedWindows.Num() == 1 && IsValid(PlayerController))
		{
			PlayerController->SetUIControlMode(true);
		}
		return Window;
	}

	return nullptr;
}

void UGSBPlayerOverlay::CloseWindow(UGSBWindow* Window)
{
	Super::CloseWindow(Window);

	if (OpenedWindows.Num() == 0)
	{
		PlayerController->SetUIControlMode(false);
	}
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