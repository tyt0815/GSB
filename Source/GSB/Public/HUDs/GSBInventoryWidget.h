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
	virtual void UpdateWidget() override;

private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* WeightProgressBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WeightLimitText;
};
