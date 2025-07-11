// Fill out your copyright notice in the Description page of Project Settings.


#include "Facility/ExtensionHub.h"
#include "GSBGameInstance.h"
#include "DebugHeader.h"	

AExtensionHub::AExtensionHub()
{
	FacilityName = TEXT("확장 허브");
}

bool AExtensionHub::TryLinkPowerConsumerFacility(APowerConsumerFacility* PowerConsumerFacility)
{
	AHubLinkedFacility* HubLinkedFacility = Cast<AHubLinkedFacility>(PowerConsumerFacility);
	
	if (IsValid(HubLinkedFacility))
	{
		return Super::TryLinkPowerConsumerFacility(PowerConsumerFacility);
	}
	return false;
}

bool AExtensionHub::CanLinkToPowerProvider(IPowerProviderFacility* PowerProvider)
{
	AExtensionHub* ExtensionHub = Cast<AExtensionHub>(PowerProvider);
	return Super::CanLinkToPowerProvider(PowerProvider) &&
		!(IsValid(ExtensionHub) && ExtensionHub->GetLinkedPowerProvider() == this)
		;
}

UItemStorageComponent* AExtensionHub::GetHubStorageComponent()
{
	return IsValid(LinkedPowerProvider.GetObject()) && LinkedPowerProvider.GetObject()->Implements<UHubFacility>() ?
		Cast<IHubFacility>(LinkedPowerProvider.GetObject())->GetHubStorageComponent() : nullptr;
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
