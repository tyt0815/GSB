// Fill out your copyright notice in the Description page of Project Settings.


#include "GSBGameInstance.h"
#include "Facility/HubLinkedFacility.h"
#include "Facility/PowerDistributorLinkedFacility.h"
#include "Interfaces/PowerProviderFacility.h"
#include "Components/PowerProviderComponent.h"
#include "Items/ItemCrate.h"

void UGSBGameInstance::CleanFacilityList(TArray<AFacility*>& Facilities)
{
	TArray<AFacility*> FacilitiesToRemove;
	for (AFacility* Facility : Facilities)
	{
		if (!IsValid(Facility))
		{
			FacilitiesToRemove.Add(Facility);
		}
	}

	for (AFacility* Facility : FacilitiesToRemove)
	{
		Facilities.Remove(Facility);
	}
}

void UGSBGameInstance::SetFacilitiesPowerInfluenceVisibility(TArray<AFacility*>& Facilities, bool bVisibility)
{
	CleanFacilityList(Facilities);

	for (AFacility* Facility : Facilities)
	{
		IPowerProviderFacility* PowerProvider = Cast<IPowerProviderFacility>(Facility);
		if (IsValid(Facility) && PowerProvider)
		{
			PowerProvider->SetPowerInfluenceAreaVisibility(bVisibility);
		}
	}
}

void UGSBGameInstance::SetPowerInfluenceVisibilityByFacility(AFacility* Facility, bool bVisibility)
{
	if (Cast<AHubLinkedFacility>(Facility))
	{
		SetFacilitiesPowerInfluenceVisibility(AllHubs, bVisibility);
	}
	else if (Cast<APowerDistributorLinkedFacility>(Facility))
	{
		SetFacilitiesPowerInfluenceVisibility(AllPowerDistributor, bVisibility);
	}
}

TSubclassOf<AFacilityBuilder> UGSBGameInstance::GetDefaultFacilityBuilderClass() const
{
	return DefaultFacilityBuilderClass;
}

TSubclassOf<AItemCrate> UGSBGameInstance::GetDefaultItemCrateClass() const
{
	return DefaultItemCrateClass;
}
