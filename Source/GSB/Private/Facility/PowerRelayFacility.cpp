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
	SetPowerInfluenceAreaVisibility(false);
}

void APowerRelayFacility::OnLinkToPowerProvider_Implementation(AActor* PowerProviderActor)
{
	Super::OnLinkToPowerProvider_Implementation(PowerProviderActor);
	if (LinkedPowerProvider)
	{
		LinkedPowerProvider->UpdatePowerUsage(PowerProviderComponent->GetCurrentPowerUsage());
	}
}

void APowerRelayFacility::OnUnlinkFromPowerProvider_Implementation()
{
	Super::OnUnlinkFromPowerProvider_Implementation();

	if (LinkedPowerProvider)
	{
		LinkedPowerProvider->UpdatePowerUsage(-PowerProviderComponent->GetCurrentPowerUsage());
	}

}

int32 APowerRelayFacility::GetTotalPowerUsage() const
{
	return Super::GetTotalPowerUsage() + PowerProviderComponent->GetCurrentPowerUsage();
}

void APowerRelayFacility::TurnOff()
{
	PowerProviderComponent->UnlinkAllPowerConsumerFacility();
	Super::TurnOff();
}

void APowerRelayFacility::UpdatePowerUsage(int32 Addition)
{
	PowerProviderComponent->UpdatePowerUsage(Addition);
	if (IsLinkedToPowerProvider())
	{
		LinkedPowerProvider->UpdatePowerUsage(Addition);
	}
	UpdatePowerWidgets();
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
	SetPowerInfluenceAreaVisibility(false);
}

void APowerRelayFacility::TurnOn()
{
	Super::TurnOn();
	PowerProviderComponent->LinkFacilitiesInPowerInfluenceArea();
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