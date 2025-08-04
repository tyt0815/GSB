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

	SetPowerInfluenceAreaVisibility(false);
}

void APowerRelayFacility::OnLinkToPowerProvider_Implementation(AActor* PowerProviderActor)
{
	Super::OnLinkToPowerProvider_Implementation(PowerProviderActor);
	check(IsLinkedToPowerProvider())
	LinkedPowerProviderInterface->UpdatePowerUsage(PowerProviderComponent->GetCurrentPowerUsage());
	PowerProviderComponent->LinkFacilitiesInPowerInfluenceArea();
}

void APowerRelayFacility::OnUnlinkFromPowerProvider_Implementation()
{
	check(IsLinkedToPowerProvider());
	LinkedPowerProviderInterface->UpdatePowerUsage(-PowerProviderComponent->GetCurrentPowerUsage());

	Super::OnUnlinkFromPowerProvider_Implementation();
}

void APowerRelayFacility::PostUnlinkFromPowerProvider_Implementation()
{
	PowerProviderComponent->UnlinkAllPowerConsumerFacility();

	Super::PostUnlinkFromPowerProvider_Implementation();
}

int32 APowerRelayFacility::GetTotalPowerUsage() const
{
	return Super::GetTotalPowerUsage() + PowerProviderComponent->GetCurrentPowerUsage();
}

void APowerRelayFacility::UpdatePowerUsage(int32 Addition)
{
	PowerProviderComponent->UpdatePowerUsage(Addition);
	if (IsLinkedToPowerProvider())
	{
		LinkedPowerProviderInterface->UpdatePowerUsage(Addition);
	}
	UpdatePowerWidgets();
}


bool APowerRelayFacility::CanProvidePower()
{
	return IsLinkedToPowerProvider() && LinkedPowerProviderInterface->CanProvidePower() && IsOperating();
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

void APowerRelayFacility::PreTurnOn_Implementation()
{
	Super::PreTurnOn_Implementation();
}

void APowerRelayFacility::PostTurnOn_Implementation()
{
	Super::PostTurnOn_Implementation();
	PowerProviderComponent->LinkFacilitiesInPowerInfluenceArea();
}

void APowerRelayFacility::PreTurnOff_Implementation()
{
	Super::PreTurnOff_Implementation();
}

void APowerRelayFacility::PostTurnOff_Implementation()
{
	Super::PostTurnOff_Implementation();
}

bool APowerRelayFacility::CanLinkPowerConsumerFacility(APowerConsumerFacility* PowerConsumer)
{
	return IsLinkedToPowerProvider() && !PowerProviderComponent->IsLinked(PowerConsumer) && PowerConsumer != this;
}

bool APowerRelayFacility::TryLinkPowerConsumerFacility(APowerConsumerFacility* PowerConsumer)
{
	if (CanLinkPowerConsumerFacility(PowerConsumer))
	{
		return PowerProviderComponent->TryLinkPowerConsumerFacility(PowerConsumer);
	}
	return false;
}

void APowerRelayFacility::UnlinkPowerConsumerFacility(APowerConsumerFacility* PowerConsumer)
{
	PowerProviderComponent->UnlinkPowerConsumerFacility(PowerConsumer);
}