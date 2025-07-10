// Fill out your copyright notice in the Description page of Project Settings.


#include "Facility/ConveyorTriSplitter.h"
#include "Facility/Addon/InputPort.h"
#include "Facility/Addon/OutputPort.h"
#include "Components/RetryPrioritizedActorRequestHandlerComponent.h"
#include "Items/ItemCrate.h"
#include "DebugHeader.h"

AConveyorTriSplitter::AConveyorTriSplitter()
{
	InputPortHandler = CreateDefaultSubobject<URetryPrioritizedActorRequestHandlerComponent>(TEXT("InputPortHandler"));
	OutputPortHandler = CreateDefaultSubobject<URetryPrioritizedActorRequestHandlerComponent>(TEXT("OutputPortHandler"));
}

void AConveyorTriSplitter::RegisterInputPort(AInputPort* InputPort)
{
	InputPortHandler->RegisterActor(InputPort);
	InputPort->OnCanReceiveItemCalled.BindDynamic(this, &AConveyorTriSplitter::CanReceiveItem);
}

void AConveyorTriSplitter::RegisterOutputPort(AOutputPort* OutputPort)
{
	OutputPortHandler->RegisterActor(OutputPort);
}

void AConveyorTriSplitter::BeginPlay()
{
	Super::BeginPlay();
	InputPortHandler->OnProcess.BindDynamic(this, &AConveyorTriSplitter::HandleInputPort);
	OutputPortHandler->OnProcess.BindDynamic(this, &AConveyorTriSplitter::HandleOutputPort);
}

void AConveyorTriSplitter::TransportItem(UItemDataAsset* ItemData)
{
	CurrentTransportedItemData = ItemData;
}

bool AConveyorTriSplitter::HandleInputPort(AActor* Actor)
{
	AInputPort* InputPort = Cast<AInputPort>(Actor);
	check(InputPort);

	InputPort->TryReceiveItem();
	if (AItemCrate* ItemCrate = Cast<AItemCrate>(InputPort->TakeReceivedItem()))
	{
		FTimerDelegate TransportDelegate;
		TransportDelegate.BindUFunction(this, TEXT("TransportItem"), ItemCrate->GetItemData());
		GetWorldTimerManager().SetTimer(TransportTimerHandler, TransportDelegate, 1, false);
		ItemCrate->Destroy();
		return true;
	}

	return false;
}

bool AConveyorTriSplitter::HandleOutputPort(AActor* Actor)
{
	AOutputPort* OutputPort = Cast<AOutputPort>(Actor);
	check(OutputPort);
	if (CurrentTransportedItemData && OutputPort->IsConnectedToReceiver())
	{
		if (OutputPort->TryCreateAndSendItemCrate(CurrentTransportedItemData))
		{
			CurrentTransportedItemData = nullptr;
			return true;
		}
	}
	return false;
}

bool AConveyorTriSplitter::CanReceiveItem(const AInputPort* InputPort)
{
	return !GetWorldTimerManager().IsTimerActive(TransportTimerHandler) && !CurrentTransportedItemData;
}
