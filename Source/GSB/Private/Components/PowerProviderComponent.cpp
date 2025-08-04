// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PowerProviderComponent.h"
#include "Facility/PowerConsumerFacility.h"
#include "DebugHeader.h"

UPowerProviderComponent::UPowerProviderComponent()
{
	SetCollisionProfileName(TEXT("PowerInfluenceArea"));
}

bool UPowerProviderComponent::TryLinkPowerConsumerFacility(APowerConsumerFacility* PowerConsumerFacility)
{
	if (!IsLinked(PowerConsumerFacility))
	{
		PowerConsumerFacility->PreLinkToPowerProvider();
		LinkedPowerConsumerFacilities.AddUnique(PowerConsumerFacility);
		PowerConsumerFacility->OnLinkToPowerProvider(GetOwner());
		PowerConsumerFacility->PostLinkToPowerProvider();
		return true;
	}
	return false;
}

void UPowerProviderComponent::UnlinkPowerConsumerFacility(APowerConsumerFacility* PowerConsumerFacility)
{
	if (LinkedPowerConsumerFacilities.Contains(PowerConsumerFacility))
	{
		if (IsValid(PowerConsumerFacility))
		{
			PowerConsumerFacility->PreUnlinkFromPowerProvider();
			LinkedPowerConsumerFacilities.Remove(PowerConsumerFacility);
			PowerConsumerFacility->OnUnlinkFromPowerProvider();
			PowerConsumerFacility->PostUnlinkFromPowerProvider();
		}
		else
		{
			LinkedPowerConsumerFacilities.Remove(PowerConsumerFacility);
		}
	}	
}

void UPowerProviderComponent::LinkFacilitiesInPowerInfluenceArea()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1)); // FacilityMesh
	TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> HitResults;
	BoxTraceMultiFromGridBoundsForObjects(ObjectTypes, ActorsToIgnore, HitResults);
	
	for (const FHitResult& HitResult : HitResults)
	{
		if (APowerConsumerFacility* PowerConsumer = Cast<APowerConsumerFacility>(HitResult.GetActor()))
		{
			if (!PowerConsumer->IsLinkedToPowerProvider())
			{
				PowerConsumer->TryLinkToPowerProvider(Cast<IPowerProviderFacility>(GetOwner()));
			}
		}
	}
}

void UPowerProviderComponent::UnlinkAllPowerConsumerFacility()
{
	for (int i = LinkedPowerConsumerFacilities.Num() - 1; i >= 0; --i)
	{
		UnlinkPowerConsumerFacility(LinkedPowerConsumerFacilities[i]);
	}
}

void UPowerProviderComponent::SetPowerInfluenceAreaVisibility(bool bVisibility, bool bPropagateToChildren)
{
	if (IsValid(PowerInfluenceAreaMeshComponent))
	{
		PowerInfluenceAreaMeshComponent->SetVisibility(bVisibility, bPropagateToChildren);
	}
}

bool UPowerProviderComponent::IsLinked(APowerConsumerFacility* PowerConsumerFacility)
{
	return IsValid(PowerConsumerFacility) && LinkedPowerConsumerFacilities.Contains(PowerConsumerFacility);
}
