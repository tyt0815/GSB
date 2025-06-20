// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/PowerDistributorLinkedFacility.h"
#include "Actors/PowerDistributor.h"

bool APowerDistributorLinkedFacility::TryLinkToPowerProvider(IPowerProviderFacility* PowerProvider)
{
    APowerDistributor* PowerDistributor = Cast<APowerDistributor>(PowerProvider);
    if (IsValid(PowerDistributor))
    {
        return Super::TryLinkToPowerProvider(PowerProvider);
    }
    return false;
}
