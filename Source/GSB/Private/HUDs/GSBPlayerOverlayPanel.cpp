// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBPlayerOverlayPanel.h"
#include "HUDs/GSBInteractionList.h"
#include "HUDs/GSBPowerCapacity.h"
#include "HUDs/GSBFacilityQuickSlotList.h"
#include "Components/WidgetSwitcher.h"
#include "PlayerController/GSBPlayerController.h"
#include "Facility/CentralHub.h"
#include "SubSystems/GSBFacilitySubsystem.h"

void UGSBPlayerOverlayPanel::NativeConstruct()
{
	Super::NativeConstruct();
	
	OnWindowOpened.AddDynamic(this, &UGSBPlayerOverlayPanel::HandleOnWindowOpened);
	OnWindowClosed.AddDynamic(this, &UGSBPlayerOverlayPanel::HandleOnWindowClosed);
}

void UGSBPlayerOverlayPanel::InitializeOverlayPanel()
{
	HideInteractionList();

	if (UWorld* World = GetWorld())
	{
		if (UGameInstance* GameInst = World->GetGameInstance())
		{
			if (UGSBFacilitySubsystem* FacilitySubsystem = GameInst->GetSubsystem<UGSBFacilitySubsystem>())
			{
				if (ACentralHub * CentralHub = FacilitySubsystem->GetCentralHub())
				{
					CentralHub->OnUpdatePowerUsage.AddDynamic(PowerCapacity, &UGSBPowerCapacity::UpdatePowerUsage);
					CentralHub->OnUpdatePowerCapacity.AddDynamic(PowerCapacity, &UGSBPowerCapacity::UpdatePowerCapacity); 
					CentralHub->UpdatePowerUsage(0);
					CentralHub->UpdatePowerCapacity(0);
				}
				else
				{
					TRACE_SCREEN_LOG(TEXT("CentralHub가 nullptr 입니다."));
				}
			}
		}
	}
	
}

void UGSBPlayerOverlayPanel::ShowInteractionList()
{
	if (IsValid(InteractionListWidget))
	{
		InteractionListWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void UGSBPlayerOverlayPanel::HideInteractionList()
{
	if (IsValid(InteractionListWidget))
	{
		InteractionListWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UGSBPlayerOverlayPanel::UpdateInteractionFocusing(int32 Index)
{
	if (IsValid(InteractionListWidget))
	{
		InteractionListWidget->UpdateFocusing(Index);
	}
}

void UGSBPlayerOverlayPanel::UpdateInteractionList(const TArray<FString>& Descriptions)
{
	if (IsValid(InteractionListWidget))
	{
		InteractionListWidget->UpdateInteractionList(Descriptions);
	}
}

void UGSBPlayerOverlayPanel::SwitchToBuildModeUI()
{
	if (IsValid(ConstructableFacilityQuickSlotList))
	{
		ConstructableFacilityQuickSlotList->SyncronizeFacilityQuickSlots();
		ConstructableFacilityQuickSlotList->SetVisibility(ESlateVisibility::Visible);
	}

	if (IsValid(ModeShortcutSwitcher))
	{
		ModeShortcutSwitcher->SetActiveWidgetIndex(1);
	}

	if (IsValid(WindowShortcutSwitcher))
	{
		WindowShortcutSwitcher->SetActiveWidgetIndex(1);
	}
}

void UGSBPlayerOverlayPanel::SwitchToCombatModeUI()
{
	if (IsValid(ConstructableFacilityQuickSlotList))
	{
		ConstructableFacilityQuickSlotList->SetVisibility(ESlateVisibility::Hidden);
	}

	if (IsValid(ModeShortcutSwitcher))
	{
		ModeShortcutSwitcher->SetActiveWidgetIndex(0);
	}

	if (IsValid(WindowShortcutSwitcher))
	{
		WindowShortcutSwitcher->SetActiveWidgetIndex(0);
	}
}

void UGSBPlayerOverlayPanel::SwitchToTopViewModeUI()
{
	if (IsValid(ConstructableFacilityQuickSlotList))
	{
		ConstructableFacilityQuickSlotList->SetVisibility(ESlateVisibility::Hidden);
	}

	if (IsValid(ModeShortcutSwitcher))
	{
		ModeShortcutSwitcher->SetActiveWidgetIndex(2);
	}

	if (IsValid(WindowShortcutSwitcher))
	{
		WindowShortcutSwitcher->SetActiveWidgetIndex(1);
	}
}

void UGSBPlayerOverlayPanel::HandleOnWindowOpened(UGSBWindow* Window)
{
	if (AGSBPlayerController* PC = GetOwningPlayer<AGSBPlayerController>())
	{
		if (OpenedWindows.Num() == 1)
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

void UGSBPlayerOverlayPanel::HandleOnWindowClosed(UGSBWindow* Window)
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
	}
}
