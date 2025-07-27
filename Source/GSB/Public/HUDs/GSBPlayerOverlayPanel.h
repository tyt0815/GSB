// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBOverlayPanel.h"
#include "GSBPlayerOverlayPanel.generated.h"

class UGSBInteractionList;

UCLASS()
class GSB_API UGSBPlayerOverlayPanel : public UGSBOverlayPanel
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	virtual void InitializeOverlayPanel() override;

public:
	void ShowInteractionList();

	void HideInteractionList();

	void UpdateInteractionFocusing(int32 Index);

	void UpdateInteractionList(const TArray<FString>& Descriptions);

	void SwitchToBuildModeUI();

	void SwitchToCombatModeUI();

	void SwitchToTopViewModeUI();

protected:
	UFUNCTION()
	virtual void HandleOnWindowOpened(UGSBWindow* Window);

	UFUNCTION()
	virtual void HandleOnWindowClosed(UGSBWindow* Window);

private:
	UPROPERTY(meta = (BindWidget))
	UGSBInteractionList* InteractionListWidget;

	UPROPERTY(meta = (BindWidget))
	class UGSBFacilityQuickSlotList* ConstructableFacilityQuickSlotList;

	UPROPERTY(meta = (BindWidget))
	class UGSBPowerCapacity* PowerCapacity;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* ModeShortcutSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WindowShortcutSwitcher;
};
