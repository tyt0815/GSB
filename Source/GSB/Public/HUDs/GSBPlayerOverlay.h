// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBOverlay.h"
#include "HUDs/GSBWindowWidget.h"
#include "GSBPlayerOverlay.generated.h"

class UGSBInteractionList;
class AGSBPlayerController;

UCLASS()
class GSB_API UGSBPlayerOverlay : public UGSBOverlay
{
	GENERATED_BODY()
public:
	virtual void CloseWindow_Internal(UGSBWindowWidget* WindowWidget) override;

protected:
	virtual UGSBWindowWidget* OpenWindow_Internal(UObject* InTargetObject) override;

public:
	virtual void InitializeOverlay() override;

	void ShowInteractionList();

	void HideInteractionList();

	void UpdateInteractionFocusing(int32 Index);

	void UpdateInteractionList(const TArray<FString>& Descriptions);

	void UpdatePowerCapacity(int32 CurrentPowerUsage, int32 TotalPowerCapacity);
	
private:

	UPROPERTY(meta = (BindWidget))
	UGSBInteractionList* InteractionListWidget;

	UPROPERTY(meta = (BindWidget))
	class UGSBPowerCapacity* PowerCapacity;

	AGSBPlayerController* PlayerController;
};
