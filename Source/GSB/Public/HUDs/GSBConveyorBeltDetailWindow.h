// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBFacilityDetailWindow.h"
#include "GSBConveyorBeltDetailWindow.generated.h"

/**
 * 
 */
UCLASS()
class GSB_API UGSBConveyorBeltDetailWindow : public UGSBFacilityDetailWindow
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	class UButton* ChainDeconstructButton;

	UFUNCTION()
	void DeconstructConnectedConveyorChain();
};
