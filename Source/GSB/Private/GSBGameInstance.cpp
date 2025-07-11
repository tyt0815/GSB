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

TSubclassOf<AActor> UGSBGameInstance::GetActorClass(const FName& Name) const
{
	if (ActorClasses.Contains(Name))
	{
		return ActorClasses[Name];
	}
	TRACE_SCREEN_LOG(TEXT("존재하지 않는 공용 Actor 클래스: ") + Name.ToString());
	return nullptr;
}

TSubclassOf<UUserWidget> UGSBGameInstance::GetUserWidgetClass(const FName& Name) const
{
	if (UserWidgetClasses.Contains(Name))
	{
		return UserWidgetClasses[Name];
	}
	TRACE_SCREEN_LOG(TEXT("존재하지 않는 공용 UserWidget 클래스: ") + Name.ToString());
	return nullptr;
}

UMaterialInterface* UGSBGameInstance::GetMaterialInterface(const FName& Name) const
{
	if (MaterialInterfaces.Contains(Name))
	{
		return MaterialInterfaces[Name];
	}
	
	TRACE_SCREEN_LOG(TEXT("존재하지 않는 공용 MaterialInterface: ") + Name.ToString());

	return nullptr;
}
