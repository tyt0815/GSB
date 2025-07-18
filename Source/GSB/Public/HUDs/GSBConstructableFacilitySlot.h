// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GSBConstructableFacilitySlot.generated.h"

class UGSBFacilityDataAsset;

UCLASS()
class GSB_API UGSBConstructableFacilitySlot : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

public:
	void UpdateFacilityData(UGSBFacilityDataAsset* InFacilityData);
	
private:
	UFUNCTION()
	void PreviewFacility();

	UPROPERTY(meta = (BindWidget))
	class UButton* SlotButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* FacilityName;

	UGSBFacilityDataAsset* FacilityData;
};
