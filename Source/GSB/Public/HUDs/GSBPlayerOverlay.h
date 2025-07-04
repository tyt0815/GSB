// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBOverlay.h"
#include "HUDs/GSBWindowWidget.h"
#include "GSBPlayerOverlay.generated.h"

class UGSBInteractionWidget;
class AGSBPlayerController;

UCLASS()
class GSB_API UGSBPlayerOverlay : public UGSBOverlay
{
	GENERATED_BODY()
public:
	virtual void CloseWindow(UGSBWindowWidget* WindowWidget) override;

protected:
	virtual UGSBWindowWidget* OpenWindow() override;

public:
	virtual void InitializeOverlay() override;

	void ShowInteractionList();

	void HideInteractionList();

	void UpdateInteractionFocusing(int32 Index);

	void UpdateInteractionList(const TArray<FString>& Descriptions);
	
private:

	UPROPERTY(meta = (BindWidget))
	UGSBInteractionWidget* InteractionListWidget;

	AGSBPlayerController* PlayerController;
};
