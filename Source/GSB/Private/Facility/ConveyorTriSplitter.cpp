// Fill out your copyright notice in the Description page of Project Settings.


#include "Facility/ConveyorTriSplitter.h"
#include "Facility/Addon/InputPort.h"
#include "Facility/Addon/OutputPort.h"
#include "Components/RetryPrioritizedActorRequestHandlerComponent.h"
#include "Components/ItemStorageComponent.h"
#include "Items/ItemCrate.h"

AConveyorTriSplitter::AConveyorTriSplitter()
{
	InputPortHandler = CreateDefaultSubobject<URetryPrioritizedActorRequestHandlerComponent>(TEXT("InputPortHandler"));
	OutputPortHandler = CreateDefaultSubobject<URetryPrioritizedActorRequestHandlerComponent>(TEXT("OutputPortHandler"));
	StorageComponent = CreateDefaultSubobject<UItemStorageComponent>(TEXT("Storage"));
	StorageComponent->SetStorageSize(1);
}

void AConveyorTriSplitter::RegisterInputPort(AInputPort* InputPort)
{
	InputPortHandler->RegisterActor(InputPort);
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

bool AConveyorTriSplitter::HandleInputPort(AActor* Actor)
{
	AInputPort* InputPort = Cast<AInputPort>(Actor);
	check(InputPort);

	AItemCrate* ItemCrate = Cast<AItemCrate>(InputPort->GetPendingItemFromSender());
	if (IsValid(ItemCrate))
	{
		if (StorageComponent->GetItemStack(ItemCrate->GetItemData()).Stack == 0)
		{
			InputPort->SetHasToken(true);
			bool bReceived = InputPort->TryReceiveItem();
			InputPort->SetHasToken(false);
			if (bReceived)
			{
				FItemStack ItemStack = {};
				ItemStack.ItemData = ItemCrate->GetItemData();
				ItemStack.Stack = 1;
				StorageComponent->StoreItem(ItemStack);
				return true;
			}
		}
	}

	return false;
}

bool AConveyorTriSplitter::HandleOutputPort(AActor* Actor)
{
	AOutputPort* OutputPort = Cast<AOutputPort>(Actor);
	check(OutputPort);

	if (!StorageComponent->IsEmpty() && OutputPort->IsConnectedToReceiver())
	{
		FItemStack ItemStack = StorageComponent->GetItemStackAt(0);
		ItemStack.Stack = 1;
		if (StorageComponent->UnstoreItem(ItemStack) == 1)
		{
			if (OutputPort->TryCreateAndSendItemCrate(ItemStack.ItemData))
			{
				return true;
			}
			else
			{
				StorageComponent->StoreItem(ItemStack);
			}
		}
	}

	return false;
}
