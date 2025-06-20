// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ExtensionHub.h"
#include "GSBGameInstance.h"
#include "DebugHeader.h"	

bool AExtensionHub::TryLinkPowerConsumerFacility(APowerConsumerFacility* PowerConsumerFacility)
{
	AHubLinkedFacility* HubLinkedFacility = Cast<AHubLinkedFacility>(PowerConsumerFacility);
	
	if (IsValid(HubLinkedFacility))
	{
		return Super::TryLinkPowerConsumerFacility(PowerConsumerFacility);
	}
	return false;
}

bool AExtensionHub::TryLinkToPowerProvider(IPowerProviderFacility* PowerProvider)
{
	AExtensionHub* ExtensionHub = Cast<AExtensionHub>(PowerProvider);
	if (!IsValid(ExtensionHub) || ExtensionHub->GetLinkedPowerProvider() != this)
	{
		return Super::TryLinkToPowerProvider(PowerProvider);
	}
	return false;
}

void AExtensionHub::BeginPlay()
{
	Super::BeginPlay();

	UGSBGameInstance* GameInst = Cast<UGSBGameInstance>(GetGameInstance());
	if (GameInst)
	{
		GameInst->AddHub(this);
	}
}
