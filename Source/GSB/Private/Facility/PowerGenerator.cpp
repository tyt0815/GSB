// Fill out your copyright notice in the Description page of Project Settings.


#include "Facility/PowerGenerator.h"
#include "Facility/Addon/InputPort.h"
#include "Facility/CentralHub.h"
#include "Components/ItemStorageComponent.h"
#include "Items/ItemCrate.h"
#include "Subsystems/GSBFacilitySubsystem.h"

constexpr int32 GENERATED_POWER_AMOUNT = 100;

APowerGenerator::APowerGenerator()
{
	ItemStorageComponent = CreateDefaultSubobject<UItemStorageComponent>(TEXT("ItemStorage"));
	ItemStorageComponent->SetStorageSize(1);
}

void APowerGenerator::OnLinkToPowerProvider_Implementation(AActor* PowerProviderActor)
{
	Super::OnLinkToPowerProvider_Implementation(PowerProviderActor);	
	if (!TryResumeGeneratingPower())
	{
		TryBeginGeneraingPower();
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

float APowerGenerator::GetRemainingPowerTimeProgress() const
{
	return GetWorldTimerManager().GetTimerRemaining(GeneratingTimerHandle) / GeneratingTime;
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
		return ItemCrate->GetItemData() == ConsumingItemData &&
			ItemStorageComponent->GetItemStack(ConsumingItemData).Stack < MaxStack &&
			IsConstructed();
	}
	return false;
}

void APowerGenerator::OnReceiveItem(AActor* Item, AInputPort* InputPort)
{
	Item->Destroy();
	ItemStorageComponent->StoreItem({ ConsumingItemData, 1 });
	TryBeginGeneraingPower();
}

bool APowerGenerator::TryBeginGeneraingPower()
{
	if (ItemStorageComponent->GetItemStack(ConsumingItemData).Stack > 0 && !IsGeneratingPower())
	{
		ItemStorageComponent->UnstoreItem({ ConsumingItemData, 1 });
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