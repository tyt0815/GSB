// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/PowerRelayFacility.h"
#include "DebugHeader.h"	

APowerRelayFacility::APowerRelayFacility()
{
	PowerInfluenceAreaComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PowerInfluenceArea"));
	PowerInfluenceAreaComponent->SetupAttachment(GetRootComponent());
	PowerInfluenceAreaComponent->SetCollisionProfileName(TEXT("PowerInfluenceArea"));
	PowerInfluenceAreaComponent->ComponentTags.Add(TEXT("PowerArea"));
}

void APowerRelayFacility::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	UpdatePowerInfluenceAreaScale(GridSize.Z);
}

bool APowerRelayFacility::CanProvidePower()
{
	return IsLinkedToPowerProvider() && LinkedPowerProviderInterface->CanProvidePower();
}

bool APowerRelayFacility::TryLinkPowerConsumerFacility(APowerConsumerFacility* PowerConsumer)
{
	if (IsLinkedToPowerProvider() && !IsLinkedPowerConsumerFacility(PowerConsumer) && PowerConsumer != this)
	{
		return IPowerProviderFacility::TryLinkPowerConsumerFacility(PowerConsumer);
	}
	return false;
}

void APowerRelayFacility::UnlinkPowerConsumerFacility(APowerConsumerFacility* PowerConsumer)
{
	if (IsLinkedPowerConsumerFacility(PowerConsumer))
	{
		IPowerProviderFacility::UnlinkPowerConsumerFacility(PowerConsumer);
	}
}

void APowerRelayFacility::OnEnterWaitingState_Implementation()
{
	Super::OnEnterWaitingState_Implementation();

	LinkFacilitiesInPowerInfluenceArea();
}

void APowerRelayFacility::LinkFacilitiesInPowerInfluenceArea()
{
	if (!IsLinkedToPowerProvider())
	{
		return;
	}

	IPowerProviderFacility::LinkFacilitiesInPowerInfluenceArea();
}

bool APowerRelayFacility::TryLinkToPowerProvider(IPowerProviderFacility* PowerProvider)
{
	if (Super::TryLinkToPowerProvider(PowerProvider))
	{
		LinkFacilitiesInPowerInfluenceArea();
		return true;
	}
	return false;
}

void APowerRelayFacility::UnlinkFromPowerProvider()
{
	Super::UnlinkFromPowerProvider();

	
	for (APowerConsumerFacility* Facility : LinkedPowerConsumerFacilities)
	{
		if (IsValid(Facility))
		{
			Facility->UnlinkFromPowerProvider();
		}
	}
}

void APowerRelayFacility::UpdatePowerUsage(int32 Addition)
{
	if (IsLinkedToPowerProvider())
	{
		LinkedPowerProviderInterface->UpdatePowerUsage(Addition);
	}
}
