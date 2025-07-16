// Fill out your copyright notice in the Description page of Project Settings.


#include "Facility/PowerGenerator.h"
#include "Facility/Addon/InputPort.h"
#include "Facility/CentralHub.h"
#include "Subsystems/GSBFacilitySubsystem.h"
#include "Items/ItemCrate.h"

constexpr int32 GENERATED_POWER_AMOUNT = 100;

void APowerGenerator::OnLinkToPowerProvider_Implementation(AActor* PowerProviderActor)
{
	Super::OnLinkToPowerProvider_Implementation(PowerProviderActor);	
	if (!TryBeginGeneraingPower())
	{
		TryResumeGeneratingPower();
	}
}

void APowerGenerator::OnUnlinkFromPowerProvider_Implementation()
{
	PauseGeneratingPower();
	Super::OnUnlinkFromPowerProvider_Implementation();
}

void APowerGenerator::RegisterInputPort(AInputPort* InInputPort)
{
	ConnectedInputPort = InInputPort;
	ConnectedInputPort->OnCanReceiveItemCalled.BindDynamic(this, &APowerGenerator::CanReceiveItem);
	ConnectedInputPort->OnReceivedItemDelegate.AddDynamic(this, &APowerGenerator::OnReceiveItem);
}

ACentralHub* APowerGenerator::GetCentralHub() const
{
	return UGSBFacilitySubsystem::Get(this)->GetCentralHub();
}

void APowerGenerator::AddPowerToCentralHub()
{
	ACentralHub* CentralHub = GetCentralHub();
	CentralHub->UpdatePowerCapacity(GENERATED_POWER_AMOUNT);
}

void APowerGenerator::SubtractPowerToCentralHub()
{
	ACentralHub* CentralHub = GetCentralHub();
	CentralHub->UpdatePowerCapacity(-GENERATED_POWER_AMOUNT);
}

bool APowerGenerator::CanReceiveItem(const AInputPort* InputPort)
{
	if (AItemCrate* ItemCrate = Cast<AItemCrate>(InputPort->GetPendingItemFromSender()))
	{
		return ItemCrate->GetItemData() == ConsumingItemData && StoredItemStack < MaxStack;
	}
	return false;
}

void APowerGenerator::OnReceiveItem(AActor* Item, AInputPort* InputPort)
{
	Item->Destroy();
	++StoredItemStack;
	TryBeginGeneraingPower();
}

bool APowerGenerator::TryBeginGeneraingPower()
{
	if (StoredItemStack > 0 && !IsGeneratingPower())
	{
		--StoredItemStack;
		AddPowerToCentralHub();
		FTimerDelegate EndGeneratingPowerDelegate;
		EndGeneratingPowerDelegate.BindUFunction(this, TEXT("EndGeneratingPower"));
		GetWorldTimerManager().SetTimer(GeneratingTimerHandle, EndGeneratingPowerDelegate, GeneratingTime, false);
	}
	return false;
}

void APowerGenerator::EndGeneratingPower()
{
	GetWorldTimerManager().ClearTimer(GeneratingTimerHandle);
	SubtractPowerToCentralHub();
	TryBeginGeneraingPower();
}

void APowerGenerator::PauseGeneratingPower()
{
	if (GetWorldTimerManager().IsTimerActive(GeneratingTimerHandle))
	{
		GetWorldTimerManager().PauseTimer(GeneratingTimerHandle);
		SubtractPowerToCentralHub();
	}
}

bool APowerGenerator::TryResumeGeneratingPower()
{
	if (GetWorldTimerManager().IsTimerPaused(GeneratingTimerHandle))
	{
		GetWorldTimerManager().UnPauseTimer(GeneratingTimerHandle);
		AddPowerToCentralHub();
		return true;
	}
	return false;
}

bool APowerGenerator::IsGeneratingPower() const
{
	return GetWorldTimerManager().IsTimerActive(GeneratingTimerHandle);
}
