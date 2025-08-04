// Fill out your copyright notice in the Description page of Project Settings.


#include "Facility/PowerDistributor.h"
#include "Facility/PowerDistributorLinkedFacility.h"
#include "SubSystems/GSBFacilitySubsystem.h"
#include "DebugHeader.h"

APowerDistributor::APowerDistributor()
{
}

bool APowerDistributor::CanLinkPowerConsumerFacility(APowerConsumerFacility* PowerConsumer)
{
	return Super::CanLinkPowerConsumerFacility(PowerConsumer) && Cast<APowerDistributorLinkedFacility>(PowerConsumer) != nullptr;
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
