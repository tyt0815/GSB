// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBOverlay.h"
#include "GSBPlayerOverlay.generated.h"



UCLASS()
class GSB_API UGSBPlayerOverlay : public UGSBOverlay
{
	GENERATED_BODY()

public:
	void HideInteractionList();

	void ShowInteractionList();

	void UpdateInteractionList(const TArray<FString>& Descriptions);

	void UpdateInteractionFocusing(int32 Index);

	void SwitchToCombatModeUI();

	void SwitchToBuildModeUI();

	class UGSBPlayerOverlayPanel* GetPlayerOverlayPanel() const;
};
