// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/GSBFacilitySubsystem.h"
#include "Facility/HubLinkedFacility.h"
#include "Facility/PowerDistributorLinkedFacility.h"
#include "Facility/CentralHub.h"
#include "Facility/PowerDistributor.h"
#include "DebugHeader.h"

UGSBFacilitySubsystem* UGSBFacilitySubsystem::Get(const UObject* WorldContext)
{	
	return WorldContext->GetWorld()->GetGameInstance()->GetSubsystem<UGSBFacilitySubsystem>();
}

void UGSBFacilitySubsystem::SetFacilityPowerInfluenceVisibility(TScriptInterface<IPowerProviderFacility> PowerProvider, bool bVisibility)
{
	if (IsValid(PowerProvider.GetObject()))
	{
		PowerProvider->SetPowerInfluenceAreaVisibility(bVisibility);
	}
}

void UGSBFacilitySubsystem::SetAllHubsPowerInfluenceVisibility(bool bVisibility)
{
	CleanInvalidHubsInArray();

	for (TScriptInterface<IHubFacility>& Hub : AllHubs)
	{
		SetFacilityPowerInfluenceVisibility(Hub.GetObject(), bVisibility);
	}
}

void UGSBFacilitySubsystem::SetAllPowerDistributorsPowerInfluenceVisibility(bool bVisibility)
{
	CleanInvalidPowerDistributorsInArray();

	for (APowerDistributor* PowerDistributor : AllPowerDistributor)
	{
		SetFacilityPowerInfluenceVisibility(PowerDistributor, bVisibility);
	}
}

void UGSBFacilitySubsystem::SetPowerInfluenceVisibilityByFacility(AFacility* Facility, bool bVisibility)
{
	if (Cast<AHubLinkedFacility>(Facility))
	{
		SetAllHubsPowerInfluenceVisibility(bVisibility);
	}
	else if (Cast<APowerDistributorLinkedFacility>(Facility))
	{
		SetAllPowerDistributorsPowerInfluenceVisibility(bVisibility);
	}
}

void UGSBFacilitySubsystem::SetPowerInfluenceVisibility(bool bVisibility)
{
	SetAllHubsPowerInfluenceVisibility(bVisibility);
	SetAllPowerDistributorsPowerInfluenceVisibility(bVisibility);
}

void UGSBFacilitySubsystem::SetCentralHub(ACentralHub* InCentralHub)
{
	if (IsValid(CentralHub))
	{
		TRACE_SCREEN_LOG(TEXT("레벨에 CentralHub클래스는 하나만 존재해야 합니다."));
	}

	CentralHub = InCentralHub;
}

void UGSBFacilitySubsystem::CleanInvalidHubsInArray()
{
	for (int i = AllHubs.Num() - 1; i >= 0; --i)
	{
		if (!IsValid(AllHubs[i].GetObject()))
		{
			AllHubs.RemoveAt(i);
		}
	}
}

void UGSBFacilitySubsystem::CleanInvalidPowerDistributorsInArray()
{
	for (int i = AllPowerDistributor.Num() - 1; i >= 0; --i)
	{
		if (!IsValid(AllPowerDistributor[i]))
		{
			AllPowerDistributor.RemoveAt(i);
		}
	}
}
