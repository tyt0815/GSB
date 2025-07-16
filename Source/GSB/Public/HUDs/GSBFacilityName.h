// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GSBFacilityName.generated.h"

/**
 * 
 */
UCLASS()
class GSB_API UGSBFacilityName : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetFacilityName(const FText InFacilityName);

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* FacilityName;
};
