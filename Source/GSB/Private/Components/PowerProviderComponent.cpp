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
	TArray<AActor*> Facilities;
	GetFacilitiesInPowerInfluenceArea(Facilities);
	for (AActor* Facility : Facilities)
	{
		APowerConsumerFacility* PowerConsumer = Cast<APowerConsumerFacility>(Facility);
		if (IsValid(PowerConsumer) && !PowerConsumer->IsLinkedToPowerProvider())
		{
			IPowerProviderFacility* a = Cast<IPowerProviderFacility>(GetOwner());
			PowerConsumer->TryLinkToPowerProvider(a);
		}
	}
}

void UPowerProviderComponent::UnlinkAllPowerConsumerFacility()
{
	TArray<APowerConsumerFacility*> LinkedFacilitiesCopy = LinkedPowerConsumerFacilities;
	for (APowerConsumerFacility* LinkedFacility : LinkedFacilitiesCopy)
	{
		LinkedFacility->UnlinkFromPowerProvider();
	}
}

bool UPowerProviderComponent::IsLinkedPowerConsumerFacility(APowerConsumerFacility* PowerConsumerFacility)
{
	return IsValid(PowerConsumerFacility) && LinkedPowerConsumerFacilities.Contains(PowerConsumerFacility);
}

void UPowerProviderComponent::GetFacilitiesInPowerInfluenceArea(TArray<AActor*>& Facilities)
{
	Facilities.Empty();
	GetOverlappingActors(Facilities, AFacility::StaticClass());
}

void UPowerProviderComponent::SetPowerInfluenceAreaVisibility(bool bVisibility, bool bPropagateToChildren)
{
	if (IsValid(PowerInfluenceAreaMeshComponent))
	{
		PowerInfluenceAreaMeshComponent->SetVisibility(bVisibility, bPropagateToChildren);
	}
}