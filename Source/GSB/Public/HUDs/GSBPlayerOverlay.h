// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBOverlay.h"
#include "GSBPlayerOverlay.generated.h"

class UGSBInteractionList;
class AGSBPlayerController;

UCLASS()
class GSB_API UGSBPlayerOverlay : public UGSBOverlay
{
	GENERATED_BODY()
public:
	virtual void InitializeOverlay() override;

	virtual UGSBWindow* OpenWindow(TSubclassOf<UGSBWindow> WindowClass, const FName& WindowName) override;

	virtual void CloseWindow(UGSBWindow* Window) override;

public:
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
