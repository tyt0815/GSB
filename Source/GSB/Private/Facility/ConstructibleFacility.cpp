// Fill out your copyright notice in the Description page of Project Settings.


#include "Facility/ConstructibleFacility.h"
#include "Interfaces/PowerProviderFacility.h"
#include "DebugHeader.h"

AConstructibleFacility::AConstructibleFacility()
{
}

void AConstructibleFacility::BeginPlay()
{
	Super::BeginPlay();

	BeginConstruction();
}

bool AConstructibleFacility::IsConstructed() const
{
	return !GetWorldTimerManager().IsTimerActive(ConstructionTimer);
}

float AConstructibleFacility::GetConstructionProgress() const
{
	return GetWorldTimerManager().GetTimerElapsed(ConstructionTimer) / ConstructionTime;
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
void AConstructibleFacility::CompleteConstruction_Implementation()
{
}