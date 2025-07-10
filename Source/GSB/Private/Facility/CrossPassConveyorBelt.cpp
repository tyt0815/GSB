// Fill out your copyright notice in the Description page of Project Settings.


#include "Facility/CrossPassConveyorBelt.h"
#include "Facility/Addon/InputPort.h"
#include "Facility/Addon/OutputPort.h"
#include "Components/RetryPrioritizedActorRequestHandlerComponent.h"
#include "Items/ItemCrate.h"
#include "DebugHeader.h"

ACrossPassConveyorBelt::ACrossPassConveyorBelt()
{
	InputPortHandler = CreateDefaultSubobject<URetryPrioritizedActorRequestHandlerComponent>(TEXT("InputPortHandler"));
	OutputPortHandler = CreateDefaultSubobject<URetryPrioritizedActorRequestHandlerComponent>(TEXT("OutputPortHandler"));
}

void ACrossPassConveyorBelt::RegisterInputPort(AInputPort* InputPort)
{
	InputPortHandler->RegisterActor(InputPort);

	InputPort->OnCanReceiveItemCalled.BindDynamic(this, &ACrossPassConveyorBelt::CanReceiveItem);

	FName Direction = GetPortDirectionByTag(InputPort);

	FItemPortSet& ItemPortPair = PortSet.FindOrAdd(Direction);
	if (!IsValid(ItemPortPair.InputPort))
	{
		ItemPortPair.InputPort = InputPort;
	}
	else
	{
		TRACE_SCREEN_LOG(Direction.ToString() + TEXT(" InputPort를 하나만 지정해 주세요."));
	}
}

void ACrossPassConveyorBelt::RegisterOutputPort(AOutputPort* OutputPort)
{
	OutputPortHandler->RegisterActor(OutputPort);

	FName Direction = GetPortDirectionByTag(OutputPort);

	FItemPortSet& ItemPortPair = PortSet.FindOrAdd(Direction);
	if (!IsValid(ItemPortPair.OutputPort))
	{
		ItemPortPair.OutputPort = OutputPort;
	}
	else
	{
		TRACE_SCREEN_LOG(Direction.ToString() + TEXT(" OutputPort를 하나만 지정해 주세요."));
	}
}

void ACrossPassConveyorBelt::BeginPlay()
{
	Super::BeginPlay();

	InputPortHandler->OnProcess.BindDynamic(this, &ACrossPassConveyorBelt::HandleInputPort);
	OutputPortHandler->OnProcess.BindDynamic(this, &ACrossPassConveyorBelt::HandleOutputPort);
}

void ACrossPassConveyorBelt::GetPayloadAndTransportTimerHandleByDirection(const FName& Direction, UItemDataAsset*& Payload, FTimerHandle*& TransportTimerHandle)
{
	if (IsVerticalPort(Direction))
	{
		Payload = VerticalPayload;
		TransportTimerHandle = &VerticalTransportTimerHandle;
	}
	else
	{
		Payload = HorizontalPayload;
		TransportTimerHandle = &HorizontalTransportTimerHandle;
	}
}

void ACrossPassConveyorBelt::SetPayloadByDirection(const FName& Direction, UItemDataAsset* Payload)
{
	PortSet.Find(Direction)->Payload = Payload;
	if (IsVerticalPort(Direction))
	{
		VerticalPayload = Payload;
	}
	else
	{
		HorizontalPayload = Payload;
	}
}

bool ACrossPassConveyorBelt::HandleInputPort(AActor* Actor)
{
	AInputPort* InputPort = Cast<AInputPort>(Actor);
	check(InputPort);

	FName Direction = GetPortDirectionByTag(InputPort);
	UItemDataAsset* Payload;
	FTimerHandle* TransportTimerHandle;
	GetPayloadAndTransportTimerHandleByDirection(Direction, Payload, TransportTimerHandle);

	InputPort->TryReceiveItem();
	if (AItemCrate* ItemCrate = Cast<AItemCrate>(InputPort->TakeReceivedItem()))
	{
		FTimerDelegate TransportDelegate;
		TransportDelegate.BindUFunction(this, TEXT("TransportItem"), ItemCrate->GetItemData(), InputPort);
		GetWorldTimerManager().SetTimer(*TransportTimerHandle, TransportDelegate, 1, false);
		ItemCrate->Destroy();
		return true;
	}

	return false;
}

bool ACrossPassConveyorBelt::HandleOutputPort(AActor* Actor)
{
	AOutputPort* OutputPort = Cast<AOutputPort>(Actor);
	check(OutputPort);

	FName Direction = GetPortDirectionByTag(OutputPort);
	UItemDataAsset* Payload = PortSet[Direction].Payload;

	if (Payload && OutputPort->IsConnectedToReceiver())
	{
		if (OutputPort->TryCreateAndSendItemCrate(Payload))
		{
			SetPayloadByDirection(Direction, nullptr);
			return true;
		}
	}
	return false;
}

FName ACrossPassConveyorBelt::GetPortDirectionByTag(const AItemPort* ItemPort) const
{
	if (ItemPort->ActorHasTag(TEXT("Forward")))
	{
		return TEXT("Forward");
	}
	else if (ItemPort->ActorHasTag(TEXT("Backward")))
	{
		return TEXT("Backward");
	}
	else if (ItemPort->ActorHasTag(TEXT("Left")))
	{
		return TEXT("Left");
	}
	else if (ItemPort->ActorHasTag(TEXT("Right")))
	{
		return TEXT("Right");
	}

	TRACE_SCREEN_LOG(TEXT("Port의 태그를 지정해 주세요."));
	return TEXT("Direction_NONE");
}

bool ACrossPassConveyorBelt::IsVerticalPort(const FName Direction) const
{
	return Direction == TEXT("Forward") || Direction == TEXT("Backward");
}

bool ACrossPassConveyorBelt::CanReceiveItem(const AInputPort* InputPort)
{
	FName Direction = GetPortDirectionByTag(InputPort);
	UItemDataAsset* Payload;
	FTimerHandle* TransportTimerHandle;
	GetPayloadAndTransportTimerHandleByDirection(Direction, Payload, TransportTimerHandle);
	return !GetWorldTimerManager().IsTimerActive(*TransportTimerHandle) && !Payload;
}

void ACrossPassConveyorBelt::TransportItem(UItemDataAsset* Payload, AInputPort* InputPort)
{
	FName Direction = GetPortDirectionByTag(InputPort);

	SetPayloadByDirection(Direction, Payload);
}
