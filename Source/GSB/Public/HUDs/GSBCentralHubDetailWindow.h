// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBFacilityDetailWindow.h"
#include "GSBCentralHubDetailWindow.generated.h"

UCLASS()
class GSB_API UGSBCentralHubDetailWindow : public UGSBFacilityDetailWindow
{
	GENERATED_BODY()
public:
	virtual void OnLinkedToFacility(AFacility* Facility) override;

private:
	UPROPERTY(meta = (BindWidget))
	class UGSBHubDetail* HubDetail;
};
