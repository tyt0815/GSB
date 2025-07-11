// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBStorageWidget.h"
#include "GSBInventoryWidget.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class GSB_API UGSBInventoryWidget : public UGSBStorageWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	virtual void UpdateWidget() override;

	void UpdateWeightUI();

private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* WeightProgressBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WeightLimitText;

	UFUNCTION()
	void AddContextMenuEntry_DropItem(UGSBContextMenu* ContextMenuWidget);

	UFUNCTION()
	void HandleContextMenuEntry_DropItem(UGSBContextMenuEntry* MenuEntry);
	
	UFUNCTION()
	void HandleDialogOKButton_DropItem(UGSBConfirmationDialog* ConfirmationDialog);
};
