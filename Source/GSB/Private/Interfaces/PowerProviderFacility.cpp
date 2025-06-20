// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/PowerProviderFacility.h"
#include "Actors/PowerConsumerFacility.h"
#include "GSBDefines.h"
#include "GSBUtility.h"
#include "DebugHeader.h"

// Add default functionality here for any IPowerProviderFacility functions that are not pure virtual.

void IPowerProviderFacility::UpdatePowerInfluenceAreaScale(uint32 HeightInGridUnits)
{
	UStaticMeshComponent* PowerInfluenceAreaComponent = GetPowerInfluenceAreaComponent();
	if (PowerInfluenceAreaComponent)
	{
		FVector Scale = CalcPowerInfluenceAreaCylinderScale(GetPowerInfluenceAreaRadiusInGridUnits(), HeightInGridUnits);
		PowerInfluenceAreaComponent->SetRelativeScale3D(Scale);
	}
}

bool IPowerProviderFacility::TryLinkPowerConsumerFacility(APowerConsumerFacility* PowerConsumerFacility)
{
	if (!IsLinkedPowerConsumerFacility(PowerConsumerFacility))
	{
		PowerConsumerFacility->UnlinkFromPowerProvider();
		PowerConsumerFacility->SetLinkedPowerProviderAndInterface(this);
		LinkedPowerConsumerFacilities.Add(PowerConsumerFacility);
		UpdatePowerUsage(PowerConsumerFacility->GetPowerConsumption());
		return true;
	}
	return false;
}

void IPowerProviderFacility::UnlinkPowerConsumerFacility(APowerConsumerFacility* PowerConsumerFacility)
{
	if (LinkedPowerConsumerFacilities.Contains(PowerConsumerFacility))
	{
		PowerConsumerFacility->SetLinkedPowerProviderAndInterface(nullptr, nullptr);
		LinkedPowerConsumerFacilities.Remove(PowerConsumerFacility);
		UpdatePowerUsage(-PowerConsumerFacility->GetPowerConsumption());
	}
}

void IPowerProviderFacility::LinkFacilitiesInPowerInfluenceArea()
{
	TArray<AActor*> Facilities;
	GetFacilitiesInPowerInfluenceArea(Facilities);
	for (AActor* Facility : Facilities)
	{
		APowerConsumerFacility* PowerConsumer = Cast<APowerConsumerFacility>(Facility);
		if (IsValid(PowerConsumer) && !PowerConsumer->IsLinkedToPowerProvider())
		{
			PowerConsumer->TryLinkToPowerProvider(this);
		}
	}
}

void IPowerProviderFacility::UnlinkAllPowerConsumerFacility()
{
	TArray<APowerConsumerFacility*> LinkedFacilitiesCopy = LinkedPowerConsumerFacilities;
	for (APowerConsumerFacility* LinkedFacility : LinkedFacilitiesCopy)
	{
		LinkedFacility->UnlinkFromPowerProvider();
	}
}

bool IPowerProviderFacility::IsLinkedPowerConsumerFacility(APowerConsumerFacility* PowerConsumerFacility)
{
	return IsValid(PowerConsumerFacility) && LinkedPowerConsumerFacilities.Contains(PowerConsumerFacility);
}

void IPowerProviderFacility::GetFacilitiesInPowerInfluenceArea(TArray<AActor*>& Facilities)
{
	Facilities.Empty();
	GetPowerInfluenceAreaComponent()->GetOverlappingActors(Facilities, AFacility::StaticClass());
}