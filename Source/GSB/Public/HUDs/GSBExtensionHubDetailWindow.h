// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBPoweredFacilityDetailWindow.h"
#include "GSBExtensionHubDetailWindow.generated.h"

/**
 * 
 */
UCLASS()
class GSB_API UGSBExtensionHubDetailWindow : public UGSBPoweredFacilityDetailWindow
{
	GENERATED_BODY()
public:
	virtual void OnLinkedToFacility(AFacility* Facility) override;

private:
	UPROPERTY(meta = (BindWidget))
	class UGSBHubDetail* HubDetail;
};
