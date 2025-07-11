// Fill out your copyright notice in the Description page of Project Settings.


#include "Facility/PowerDistributor.h"
#include "Facility/PowerDistributorLinkedFacility.h"
#include "GSBGameInstance.h"
#include "DebugHeader.h"

APowerDistributor::APowerDistributor()
{
	FacilityName = TEXT("전력 분배기");
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

	UGSBGameInstance* GameInst = Cast<UGSBGameInstance>(GetGameInstance());
	if (GameInst)
	{
		GameInst->AddPowerDistributor(this);
	}
}
