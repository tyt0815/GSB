// Fill out your copyright notice in the Description page of Project Settings.


#include "Facility/PowerDistributor.h"
#include "Facility/PowerDistributorLinkedFacility.h"
#include "SubSystems/GSBFacilitySubsystem.h"
#include "DebugHeader.h"

APowerDistributor::APowerDistributor()
{
}

bool APowerDistributor::TryLinkPowerConsumerFacility(APowerConsumerFacility* PowerConsumerFacility)
{
	APowerDistributorLinkedFacility* PowerDistributorLinkedFacility = Cast<APowerDistributorLinkedFacility>(PowerConsumerFacility);
	if (PowerDistributorLinkedFacility)
	{
		return Super::TryLinkPowerConsumerFacility(PowerConsumerFacility);
	}
	return false;
}

void APowerDistributor::BeginPlay()
{
	Super::BeginPlay();

	if (UGameInstance* GameInst = GetGameInstance())
	{
		if (UGSBFacilitySubsystem* FacilityManager = Cast<UGSBFacilitySubsystem>(GameInst->GetSubsystem<UGSBFacilitySubsystem>()))
		{
			FacilityManager->AddPowerDistributor(this);
		}
	}
}
