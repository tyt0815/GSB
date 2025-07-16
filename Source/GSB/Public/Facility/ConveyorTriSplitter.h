// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Facility/ConstructibleFacility.h"
#include "Interfaces/InputPortHandler.h"
#include "Interfaces/OutputPortHandler.h"
#include "Interfaces/ChainDeconstrutableFacility.h"
#include "ConveyorTriSplitter.generated.h"

class URetryPrioritizedActorRequestHandlerComponent;
class UItemDataAsset;

UCLASS()
class GSB_API AConveyorTriSplitter : 
	public AConstructibleFacility,
	public IInputPortHandler,
	public IOutputPortHandler,
	public IChainDeconstrutableFacility
{
	GENERATED_BODY()
public:
	AConveyorTriSplitter();

	virtual void RegisterInputPort(AInputPort* InputPort);

	virtual void RegisterOutputPort(AOutputPort* OutputPort);

	virtual void ConnectFacilityAddon(AFacilityAddon* Addon) override;

	///////////////////////////////////////////////////////////////////
	// IChainDeconstrutableFacility
	///////////////////////////////////////////////////////////////////

	virtual void DeconstructConnectedConveyorChain() override;

	virtual void DeconstructConnectedReceiverConveyorChain(bool bDeconstructSelf) override;

	virtual void DeconstructConnectedSenderConveyorChain(bool bDeconstructSelf) override;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere)
	URetryPrioritizedActorRequestHandlerComponent* InputPortHandler;

	UPROPERTY(VisibleAnywhere)
	URetryPrioritizedActorRequestHandlerComponent* OutputPortHandler;

	UFUNCTION()
	void TransportItem(UItemDataAsset* ItemData);

	FTimerHandle TransportTimerHandler;

	UPROPERTY(VisibleAnywhere)
	UItemDataAsset* CurrentTransportedItemData;

	AInputPort* BackwardInputPort;

	AOutputPort* ForwardOutputPort;

	AOutputPort* LeftOutputPort;

	AOutputPort* RightOutputPort;

private:
	UFUNCTION()
	bool HandleInputPort(AActor* Actor);

	UFUNCTION()
	bool HandleOutputPort(AActor* Actor);

	UFUNCTION()
	bool CanReceiveItem(const class AInputPort* InputPort);
};
