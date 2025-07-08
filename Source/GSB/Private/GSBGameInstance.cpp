// Fill out your copyright notice in the Description page of Project Settings.


#include "GSBGameInstance.h"
#include "Facility/HubLinkedFacility.h"
#include "Facility/PowerDistributorLinkedFacility.h"
#include "Interfaces/PowerProviderFacility.h"
#include "Components/PowerProviderComponent.h"
#include "Items/ItemCrate.h"
#include "Items/DroppedItem.h"
#include "DebugHeader.h"

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

void UGSBGameInstance::SetPowerInfluenceVisibility(bool bVisibility)
{
	SetFacilitiesPowerInfluenceVisibility(AllHubs, bVisibility);
	SetFacilitiesPowerInfluenceVisibility(AllPowerDistributor, bVisibility);
}

TSubclassOf<AItemCrate> UGSBGameInstance::GetDefaultItemCrateClass() const
{
	return DefaultItemCrateClass;
}

ADroppedItem* UGSBGameInstance::SpawnDroppedItem(const FItemStack& ItemStack)
{
	if (DefaultDroppedItemClass)
	{
		if (ADroppedItem* DroppedItem = GetWorld()->SpawnActor<ADroppedItem>(DefaultDroppedItemClass))
		{
			DroppedItem->UpdateItem(ItemStack);
			return DroppedItem;
		}
	}
	else
	{
		TRACE_SCREEN_LOG(TEXT("DefaultDroppedItemClass가 nullptr 입니다."));
	}
	return nullptr;
}

AFacilityBuilder* UGSBGameInstance::SpawnFacilityBuilder()
{
	if (DefaultFacilityBuilderClass)
	{
		return GetWorld()->SpawnActor<AFacilityBuilder>(DefaultFacilityBuilderClass);
	}
	else
	{
		TRACE_SCREEN_LOG(TEXT("DefaultFacilityBuilderClass가 nullptr 입니다."));
	}
	return nullptr;
}
