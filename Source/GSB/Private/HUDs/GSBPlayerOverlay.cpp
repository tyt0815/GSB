// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBPlayerOverlay.h"
#include "HUDs/GSBPlayerOverlayPanel.h"

void UGSBPlayerOverlay::HideInteractionList()
{
	if (UGSBPlayerOverlayPanel* PlayerOverlayPanel = GetPlayerOverlayPanel())
	{
		PlayerOverlayPanel->HideInteractionList();
	}
}

void UGSBPlayerOverlay::ShowInteractionList()
{
	if (UGSBPlayerOverlayPanel* PlayerOverlayPanel = GetPlayerOverlayPanel())
	{
		PlayerOverlayPanel->ShowInteractionList();
	}
}

void UGSBPlayerOverlay::UpdateInteractionList(const TArray<FString> &Descriptions)
{
	if (UGSBPlayerOverlayPanel* PlayerOverlayPanel = GetPlayerOverlayPanel())
	{
		PlayerOverlayPanel->UpdateInteractionList(Descriptions);
	}
}

void UGSBPlayerOverlay::UpdateInteractionFocusing(int32 Index)
{
	if (UGSBPlayerOverlayPanel* PlayerOverlayPanel = GetPlayerOverlayPanel())
	{
		PlayerOverlayPanel->UpdateInteractionFocusing(Index);
	}
}

void UGSBPlayerOverlay::SwitchToCombatModeUI()
{
	if (UGSBPlayerOverlayPanel* PlayerOverlayPanel = GetPlayerOverlayPanel())
	{
		PlayerOverlayPanel->SwitchToCombatModeUI();
	}
}

void UGSBPlayerOverlay::SwitchToBuildModeUI()
{
	if (UGSBPlayerOverlayPanel* PlayerOverlayPanel = GetPlayerOverlayPanel())
	{
		PlayerOverlayPanel->SwitchToBuildModeUI();
	}
}

UGSBPlayerOverlayPanel* UGSBPlayerOverlay::GetPlayerOverlayPanel() const
{
	if (UGSBPlayerOverlayPanel* PlayerOverlayPanel = Cast<UGSBPlayerOverlayPanel>(OverlayPanel))
	{
		return PlayerOverlayPanel;
	}
	TRACE_SCREEN_LOG(TEXT("OverlayPanel은 UGSBPlayerOverlayPanel를 상속한 클래스이어야 합니다."));
	return nullptr;
}
