// Fill out your copyright notice in the Description page of Project Settings.


#include "Facility/PowerDistributorLinkedFacility.h"
#include "Facility/PowerDistributor.h"

bool APowerDistributorLinkedFacility::CanLinkToPowerProvider(IPowerProviderFacility* PowerProvider)
{
    APowerDistributor* PowerDistributor = Cast<APowerDistributor>(PowerProvider);
    return Super::CanLinkToPowerProvider(PowerProvider) && IsValid(PowerDistributor);
}
