// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PowerProviderFacility.generated.h"

constexpr float DEFAULT_CYLINDER_RADIUS = 50.0f;
constexpr float DEFAULT_CYLINDER_HALF_HEIGHT = 50.0f;

class UPowerProviderComponent;
class APowerConsumerFacility;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPowerProviderFacility : public UInterface
{
	GENERATED_BODY()
};


class GSB_API IPowerProviderFacility
{
	GENERATED_BODY()

public:
	virtual bool TryLinkPowerConsumerFacility(APowerConsumerFacility* PowerConsumerFacility) = 0;

	virtual bool CanProvidePower() = 0;

	virtual void UnlinkPowerConsumerFacility(APowerConsumerFacility* PowerConsumerFacility) = 0;

	virtual void UpdatePowerUsage(int32 Addition) = 0;

	virtual void SetPowerInfluenceAreaVisibility(bool bVisibilty) = 0;
};
