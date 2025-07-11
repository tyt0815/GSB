// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Facility/PowerConsumerFacility.h"
#include "PowerDistributorLinkedFacility.generated.h"

/**
 * 
 */
UCLASS()
class GSB_API APowerDistributorLinkedFacility : public APowerConsumerFacility
{
	GENERATED_BODY()
public:
	virtual bool CanLinkToPowerProvider(IPowerProviderFacility* PowerProvider) override;
};
