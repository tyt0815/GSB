// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Facility/ConstructibleFacility.h"
#include "Interfaces/InputPortHandler.h"
#include "Interfaces/OutputPortHandler.h"
#include "Interfaces/ChainDeconstrutableFacility.h"
#include "CrossPassConveyorBelt.generated.h"

class UItemDataAsset;
class URetryPrioritizedActorRequestHandlerComponent;
class AItemPort;
class AInputPort;
class AOutputPort;

UCLASS()
class GSB_API ACrossPassConveyorBelt : public AConstructibleFacility,
	public IInputPortHandler,
	public IOutputPortHandler,
	public IChainDeconstrutableFacility
{
	GENERATED_BODY()
public:
	ACrossPassConveyorBelt();

	virtual void RegisterInputPort(AInputPort* InputPort) override;

	virtual void RegisterOutputPort(AOutputPort* OutputPort) override;

	///////////////////////////////////////////////////////////////
	// IChainDeconstrutableFacility
	///////////////////////////////////////////////////////////////

	virtual void DeconstructConnectedConveyorChain() override;

	virtual void DeconstructConnectedReceiverConveyorChain(bool bDeconstructSelf) override {};

	virtual void DeconstructConnectedSenderConveyorChain(bool bDeconstructSelf) override {};

protected:
	virtual void BeginPlay() override;


protected:

	UPROPERTY(VisibleAnywhere)
	URetryPrioritizedActorRequestHandlerComponent* InputPortHandler;

	UPROPERTY(VisibleAnywhere)
	URetryPrioritizedActorRequestHandlerComponent* OutputPortHandler;

	void GetPayloadAndTransportTimerHandleByDirection(const FName& Direction, UItemDataAsset*& Payload, FTimerHandle*& TransportTimerHandle);

	void SetPayloadByDirection(const FName& Direction, UItemDataAsset* Payload);

	UFUNCTION()
	bool HandleInputPort(AActor* Actor);

	UFUNCTION()
	bool HandleOutputPort(AActor* Actor);

	struct FItemPortSet
	{
		AInputPort* InputPort;
		AOutputPort* OutputPort;
		UItemDataAsset* Payload;
	};

	TMap<FName, FItemPortSet> PortSet;

	FName GetPortDirectionByTag(const AItemPort* ItemPort) const;

	// 횡방향(left, right) 운송 화물
	UPROPERTY(VisibleAnywhere)
	UItemDataAsset* HorizontalPayload;

	FTimerHandle HorizontalTransportTimerHandle;

	// 종방향(forward, backward) 운송 화물
	UPROPERTY(VisibleAnywhere)
	UItemDataAsset* VerticalPayload;

	FTimerHandle VerticalTransportTimerHandle;

	bool IsVerticalPort(const FName Direction) const;

	UFUNCTION()
	bool CanReceiveItem(const class AInputPort* InputPort);

	UFUNCTION()
	void TransportItem(UItemDataAsset* Payload, AInputPort* InputPort);
};
