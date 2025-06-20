// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/PowerDistributor.h"
#include "Actors/PowerDistributorLinkedFacility.h"
#include "GSBGameInstance.h"

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
