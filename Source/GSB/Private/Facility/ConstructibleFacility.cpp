// Fill out your copyright notice in the Description page of Project Settings.


#include "Facility/ConstructibleFacility.h"
#include "Facility/Addon/FacilityAddon.h"
#include "Interfaces/PowerProviderFacility.h"
#include "DebugHeader.h"

bool AConstructibleFacility::IsOperating() const
{
	return Super::IsOperating() && IsConstructed();
}

void AConstructibleFacility::BeginPlay()
{
	Super::BeginPlay();

	InteractionComponent->AddInteractionDynamic(TEXT("철거"), this, &AConstructibleFacility::HandleDeconstructRequest);

	BeginConstruction();
}

bool AConstructibleFacility::IsConstructed() const
{
	return !IsConstructing() && !IsDeconstructing();
}

bool AConstructibleFacility::IsConstructing() const
{
	return GetWorldTimerManager().IsTimerActive(ConstructionTimer);
}

bool AConstructibleFacility::IsDeconstructing() const
{
	return GetWorldTimerManager().IsTimerActive(DeconstructionTimer);
}

float AConstructibleFacility::GetConstructionProgress() const
{
	return GetWorldTimerManager().GetTimerElapsed(ConstructionTimer) / ConstructionTime;
}

float AConstructibleFacility::GetDeconstructionProgress() const
{
	return GetWorldTimerManager().GetTimerElapsed(DeconstructionTimer) / DeconstructionTime;
}

void AConstructibleFacility::BeginConstruction_Implementation()
{
	if (ConstructionTime == 0)
	{
		CompleteConstruction();
	}
	else
	{
		FTimerDelegate ConstructionDelegate;
		ConstructionDelegate.BindUFunction(this, TEXT("CompleteConstruction"));
		GetWorldTimerManager().SetTimer(ConstructionTimer, ConstructionDelegate, ConstructionTime, false);
	}
}
void AConstructibleFacility::HandleDeconstructRequest(AActor* Interactor)
{
	if (IsConstructing())
	{
		CompleteConstruction();
	}
	else if(IsConstructed())
	{
		BeginDeconstruction();
	}
}
void AConstructibleFacility::BeginDeconstruction_Implementation()
{
	InteractionComponent->DeactivateInteraction();
	if (DeconstructionTime == 0)
	{
		CompleteDeconstruction();
	}
	else
	{
		FTimerDelegate DeconstructionDelegate;
		DeconstructionDelegate.BindUFunction(this, TEXT("CompleteDeconstruction"));
		GetWorldTimerManager().SetTimer(DeconstructionTimer, DeconstructionDelegate, DeconstructionTime, false);
	}
	
}
void AConstructibleFacility::CompleteConstruction_Implementation()
{
	GetWorldTimerManager().ClearTimer(ConstructionTimer);
	for (AFacilityAddon* Addon : ConnectedAddons)
	{
		Addon->CompleteConstruction();
	}
}

void AConstructibleFacility::CompleteDeconstruction_Implementation()
{
	Destroy();
}
