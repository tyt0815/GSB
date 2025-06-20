// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/HubLinkedFacility.h"
#include "Interfaces/HubFacility.h"
#include "Interfaces/PowerProviderFacility.h"
#include "DebugHeader.h"

bool AHubLinkedFacility::TryLinkToPowerProvider(IPowerProviderFacility* PowerProvider)
{
	IHubFacility* HubInterface = Cast<IHubFacility>(PowerProvider);
	if (HubInterface)
	{
		return Super::TryLinkToPowerProvider(PowerProvider);
	}
	return false;
}
