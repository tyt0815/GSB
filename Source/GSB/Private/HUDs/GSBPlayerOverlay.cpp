// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBPlayerOverlay.h"
#include "HUDs/GSBInteractionList.h"
#include "HUDs/GSBPowerCapacity.h"
#include "HUDs/GSBFacilityQuickSlotList.h"
#include "PlayerController/GSBPlayerController.h"

void UGSBPlayerOverlay::InitializeOverlay()
{
	HideInteractionList();
}

void UGSBPlayerOverlay::UpdatePlayerControllMode()
{
	if (AGSBPlayerController* PC = GetOwningPlayer<AGSBPlayerController>())
	{
		
		if (OpenedWindows.Num() == 0)
		{
			if (PC->IsPlayerCombatMode())
			{
				PC->SetGamePlayMode_PlayerCombatGameOnly();
			}
			else if (PC->IsPlayerBuildMode())
			{
				PC->SetGamePlayMode_PlayerBuildGameOnly();
			}
		}
		else if (OpenedWindows.Num() == 1)
		{
			if (PC->IsPlayerCombatMode())
			{
				PC->SetGamePlayMode_PlayerCombatGameAndUI();
			}
			else if (PC->IsPlayerBuildMode())
			{
				PC->SetGamePlayMode_PlayerBuildGameAndUI();
			}
		}
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

void UGSBPlayerOverlay::SwitchToBuildModeUI()
{
	ConstructableFacilityQuickSlotList->SyncronizeFacilityQuickSlots();
	ConstructableFacilityQuickSlotList->SetVisibility(ESlateVisibility::Visible);
}

void UGSBPlayerOverlay::SwitchToCombatModeUI()
{
	ConstructableFacilityQuickSlotList->SetVisibility(ESlateVisibility::Hidden);
}
