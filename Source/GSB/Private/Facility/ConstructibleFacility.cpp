// Fill out your copyright notice in the Description page of Project Settings.


#include "Facility/ConstructibleFacility.h"
#include "Facility/Addon/FacilityAddon.h"
#include "Interfaces/PowerProviderFacility.h"
#include "Components/ItemStorageComponent.h"
#include "DebugHeader.h"

bool AConstructibleFacility::IsOperating() const
{
	return Super::IsOperating() && IsConstructed();
}

void AConstructibleFacility::AddDefaultInteractions()
{
	Super::AddDefaultInteractions();
	InteractionComponent->AddInteractionDynamic(TEXT("철거"), this, &AConstructibleFacility::HandleDeconstructRequest);
}

void AConstructibleFacility::BeginPlay()
{
	Super::BeginPlay();

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

bool AConstructibleFacility::TryBeginConstruction()
{
	if (IsConstructing())
	{
		return false;
	}
	BeginConstruction();
	return true;
}

bool AConstructibleFacility::TryBeginDeconstruction()
{
	if (IsConstructed())
	{
		BeginDeconstruction();
		return true;
	}
	return false;
}

void AConstructibleFacility::BeginConstruction_Implementation()
{
	InteractionComponent->ClearInteractions();

	InteractionComponent->AddInteractionDynamic(TEXT("건설 취소"), this, &AConstructibleFacility::HandleCancelConstruction);

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
void AConstructibleFacility::HandleCancelConstruction(AActor* Interactor)
{
	Destroy();
}
void AConstructibleFacility::HandleDeconstructRequest(AActor* Interactor)
{
	if (IsConstructing())
	{
		CompleteConstruction();
	}
	else if(IsConstructed())
	{
		TryBeginDeconstruction();
	}
}
void AConstructibleFacility::BeginDeconstruction_Implementation()
{
	if (IsDeconstructing())
	{
		return;	
	}
	if (IsValid(DetailWindow))
	{
		DetailWindow->Close();
	}

	InteractionComponent->DeactivateInteraction();
	SetHighlighInteractableActor(false);
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

	InteractionComponent->ClearInteractions();
	AddDefaultInteractions();

}

void AConstructibleFacility::CompleteDeconstruction_Implementation()
{
	TArray<UItemStorageComponent*> ItemStorageComponents;
	GetComponents(ItemStorageComponents);
	for (UItemStorageComponent* ItemStorage : ItemStorageComponents)
	{
		ItemStorage->DropAllItems();
	}

	Destroy();
}
