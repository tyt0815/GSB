// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDs/GSBInteractionDescriptionWidget.h"
#include "GSBInteractionWidget.generated.h"

class UVerticalBox;

UCLASS()
class GSB_API UGSBInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateInteractionList(const TArray<FString>& Descriptions);

	void FocusDescriptionWidget(int32 Index);

	void UnfocusDescriptionWidget(int32 Index);

	void UpdateFocusing(int32 Index);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UGSBInteractionWidget")
	TSubclassOf<UGSBInteractionDescriptionWidget> InteractionDescriptionWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UGSBInteractionWidget")
	float UnfocusedOpacity = 0.5;

	
private:
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* InteractionList;

	int32 SelectedInteractionIndex = 0;
};
