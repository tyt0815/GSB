// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/PowerRelayFacility.h"
#include "Interfaces/HubFacility.h"
#include "ExtensionHub.generated.h"

UCLASS()
class GSB_API AExtensionHub : public APowerRelayFacility,  public IHubFacility
{
	GENERATED_BODY()
	
public:
	virtual bool TryLinkPowerConsumerFacility(APowerConsumerFacility* PowerConsumerFacility) override;

	virtual bool TryLinkToPowerProvider(IPowerProviderFacility* PowerProvider) override;

protected:
	virtual void BeginPlay() override;

};
