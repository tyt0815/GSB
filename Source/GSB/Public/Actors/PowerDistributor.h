// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/PowerRelayFacility.h"
#include "Interfaces/PowerProviderFacility.h"
#include "PowerDistributor.generated.h"

UCLASS()
class GSB_API APowerDistributor : public APowerRelayFacility
{
	GENERATED_BODY()
public:
	virtual bool TryLinkPowerConsumerFacility(APowerConsumerFacility* PowerConsumerFacility) override;

protected:
	virtual void BeginPlay() override;
};
