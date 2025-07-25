// Fill out your copyright notice in the Description page of Project Settings.


#include "Facility/HubLinkedFacility.h"
#include "Interfaces/HubFacility.h"
#include "Interfaces/PowerProviderFacility.h"
#include "DebugHeader.h"

bool AHubLinkedFacility::CanLinkToPowerProvider(IPowerProviderFacility* PowerProvider)
{
	IHubFacility* HubInterface = Cast<IHubFacility>(PowerProvider);
	return Super::CanLinkToPowerProvider(PowerProvider) && HubInterface;
}
