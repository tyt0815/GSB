// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GSBConstructableFacilitySlot.generated.h"

class AFacilityBuilder;
class UGSBFacilityDataAsset;

UCLASS()
class GSB_API UGSBConstructableFacilitySlot : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

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

	AFacilityBuilder* FacilityBuilder;

public:
	FORCEINLINE UGSBFacilityDataAsset* GetFacilityData() const
	{
		return FacilityData;
	}
	FORCEINLINE void SetFacilityBuilder(AFacilityBuilder* NewFacilityBuilder)
	{
		FacilityBuilder = NewFacilityBuilder;
	}
};
