// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Facility/PowerRelayFacility.h"
#include "Interfaces/HubFacility.h"
#include "ExtensionHub.generated.h"

UCLASS()
class GSB_API AExtensionHub : public APowerRelayFacility,  public IHubFacility
{
	GENERATED_BODY()
	
public:
	AExtensionHub();

	virtual bool TryLinkPowerConsumerFacility(APowerConsumerFacility* PowerConsumerFacility) override;

	virtual bool CanLinkToPowerProvider(IPowerProviderFacility* PowerProvider) override;

	virtual UItemStorageComponent* GetHubStorageComponent() override;

protected:
	virtual void BeginPlay() override;

};
