// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PowerProviderComponent.h"
#include "Facility/PowerConsumerFacility.h"
#include "DebugHeader.h"

UPowerProviderComponent::UPowerProviderComponent()
{
	SetCollisionProfileName(TEXT("PowerInfluenceArea"));
}

void UPowerProviderComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void UPowerProviderComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UPowerProviderComponent::TryLinkPowerConsumerFacility(APowerConsumerFacility* PowerConsumerFacility)
{
	if (!IsLinkedPowerConsumerFacility(PowerConsumerFacility))
	{
		LinkedPowerConsumerFacilities.AddUnique(PowerConsumerFacility);
		PowerConsumerFacility->OnLinkToPowerProvider(GetOwner());
		return true;
	}
	return false;
}

void UPowerProviderComponent::UnlinkPowerConsumerFacility(APowerConsumerFacility* PowerConsumerFacility)
{
	if (LinkedPowerConsumerFacilities.Contains(PowerConsumerFacility))
	{
		LinkedPowerConsumerFacilities.Remove(PowerConsumerFacility);
		PowerConsumerFacility->OnUnlinkFromPowerProvider();
	}
}

void UPowerProviderComponent::LinkFacilitiesInPowerInfluenceArea()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2)); // FacilityMesh
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

bool UPowerProviderComponent::IsLinkedPowerConsumerFacility(APowerConsumerFacility* PowerConsumerFacility)
{
	return IsValid(PowerConsumerFacility) && LinkedPowerConsumerFacilities.Contains(PowerConsumerFacility);
}

void UPowerProviderComponent::SetPowerInfluenceAreaVisibility(bool bVisibility, bool bPropagateToChildren)
{
	if (IsValid(PowerInfluenceAreaMeshComponent))
	{
		PowerInfluenceAreaMeshComponent->SetVisibility(bVisibility, bPropagateToChildren);
	}
}