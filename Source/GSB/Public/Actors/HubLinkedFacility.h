// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/PowerConsumerFacility.h"
#include "HubLinkedFacility.generated.h"



UCLASS()
class GSB_API AHubLinkedFacility : public APowerConsumerFacility
{
	GENERATED_BODY()
	////////////////////////////////////////////////////
	// Link to Hub
	////////////////////////////////////////////////////
	
public:
	virtual bool TryLinkToPowerProvider(IPowerProviderFacility* PowerProvider) override;
};
