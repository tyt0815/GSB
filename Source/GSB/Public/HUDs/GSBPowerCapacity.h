// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GSBPowerCapacity.generated.h"

class UTextBlock;

UCLASS()
class GSB_API UGSBPowerCapacity : public UUserWidget
{
	GENERATED_BODY()
public:
	void UpdateWidget(int32 NewPowerUsage, int32 NewPowerCapacity);

	UFUNCTION()
	void UpdatePowerUsage(int32 NewPowerUsage);

	UFUNCTION()
	void UpdatePowerCapacity(int32 NewPowerCapacity);
	
private	:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PowerCapacityText;

	int32 PowerCapacity;

	int32 PowerUsage;
};
