// Fill out your copyright notice in the Description page of Project Settings.


#include "Facility/Addon/FacilityAddon.h"
#include "Facility/Facility.h"
#include "Components/BoxComponent.h"

AFacilityAddon::AFacilityAddon()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AFacilityAddon::BeginPlay()
{
	Super::BeginPlay();
	
	if (AFacility* Facility = Cast<AFacility>(GetAttachParentActor()))
	{
		SetOwner(Facility);
		Facility->ConnectFacilityAddon(this);
	}
}

