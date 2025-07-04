// Fill out your copyright notice in the Description page of Project Settings.


#include "Facility/PowerRelayFacility.h"
#include "Components/PowerProviderComponent.h"
#include "DebugHeader.h"	

APowerRelayFacility::APowerRelayFacility()
{
	PowerProviderComponent = CreateDefaultSubobject<UPowerProviderComponent>(TEXT("PowerProvider"));
	PowerProviderComponent->SetupAttachment(GetRootComponent());
	PowerProviderComponent->SetCollisionProfileName(TEXT("PowerInfluenceArea"));

	PowerInfluenceAreaStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PowerInfluenceArea"));
	PowerInfluenceAreaStaticMeshComponent->SetupAttachment(GetRootComponent());
	PowerInfluenceAreaStaticMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
}

void APowerRelayFacility::BeginConstruction_Implementation()
{
	Super::BeginConstruction_Implementation();

	PowerProviderComponent->SetPowerInfluenceAreaVisibility(false);
}

void APowerRelayFacility::TurnOn_Implementation()
{
	Super::TurnOn_Implementation();

	PowerProviderComponent->LinkFacilitiesInPowerInfluenceArea();
}

void APowerRelayFacility::UpdatePowerUsage(int32 Addition)
{
	PowerProviderComponent->UpdatePowerUsage(Addition);

	if (IsLinkedToPowerProvider())
	{
		LinkedPowerProvider->UpdatePowerUsage(Addition);
	}
}


bool APowerRelayFacility::CanProvidePower()
{
	return IsLinkedToPowerProvider() && LinkedPowerProvider->CanProvidePower();
}

void APowerRelayFacility::SetPowerInfluenceAreaVisibility(bool bVisibilty)
{
	PowerProviderComponent->SetPowerInfluenceAreaVisibility(bVisibilty);
}

void APowerRelayFacility::BeginPlay()
{
	Super::BeginPlay();

	PowerProviderComponent->SetPowerInfluenceAreaMeshComponent(PowerInfluenceAreaStaticMeshComponent);
}

bool APowerRelayFacility::TryLinkPowerConsumerFacility(APowerConsumerFacility* PowerConsumer)
{
	if (!PowerProviderComponent->IsLinkedPowerConsumerFacility(PowerConsumer) && PowerConsumer != this)
	{
		return PowerProviderComponent->TryLinkPowerConsumerFacility(PowerConsumer);
	}
	return false;
}

void APowerRelayFacility::UnlinkPowerConsumerFacility(APowerConsumerFacility* PowerConsumer)
{
	if (PowerProviderComponent->IsLinkedPowerConsumerFacility(PowerConsumer))
	{
		PowerProviderComponent->UnlinkPowerConsumerFacility(PowerConsumer);
	}
}