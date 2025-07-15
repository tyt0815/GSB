// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBFacilityDetailWindow.h"
#include "GSBTransportFacilityDetailWindow.generated.h"

/**
 * 
 */
UCLASS()
class GSB_API UGSBTransportFacilityDetailWindow : public UGSBFacilityDetailWindow
{
	GENERATED_BODY()
	
public:
	virtual void OnLinkedToFacility(AFacility* Facility);

private:
	UPROPERTY(meta = (BindWidget))
	class UGSBChainDeconstructionButton* ChainDeconstructButton;
};
