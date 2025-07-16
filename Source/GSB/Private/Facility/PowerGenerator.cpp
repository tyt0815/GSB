// Fill out your copyright notice in the Description page of Project Settings.


#include "Facility/PowerGenerator.h"
#include "Facility/Addon/InputPort.h"
#include "Facility/CentralHub.h"
#include "Items/ItemCrate.h"

void APowerGenerator::OnLinkToPowerProvider_Implementation(AActor* PowerProviderActor)
{
	Super::OnLinkToPowerProvider_Implementation(PowerProviderActor);

	
}

void APowerGenerator::OnUnlinkFromPowerProvider_Implementation()
{

	Super::OnUnlinkFromPowerProvider_Implementation();
}

void APowerGenerator::RegisterInputPort(AInputPort* InInputPort)
{
	ConnectedInputPort = InInputPort;
	ConnectedInputPort->OnCanReceiveItemCalled.BindDynamic(this, &APowerGenerator::CanReceiveItem);
	ConnectedInputPort->OnReceivedItemDelegate.AddDynamic(this, &APowerGenerator::OnReceiveItem);
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
		
		FTimerDelegate EndGeneratingPowerDelegate;
		EndGeneratingPowerDelegate.BindUFunction(this, TEXT("EndGeneratingPower"));
		GetWorldTimerManager().SetTimer(GeneratingTimerHandle, EndGeneratingPowerDelegate, GeneratingTime, false);
	}
	return false;
}

void APowerGenerator::EndGeneratingPower()
{
	GetWorldTimerManager().ClearTimer(GeneratingTimerHandle);
	TryBeginGeneraingPower();
}

bool APowerGenerator::IsGeneratingPower() const
{
	return GetWorldTimerManager().IsTimerActive(GeneratingTimerHandle);
}
