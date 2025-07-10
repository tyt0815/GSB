// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Facility/ConstructibleFacility.h"
#include "Interfaces/InputPortHandler.h"
#include "Interfaces/OutputPortHandler.h"
#include "ConveyorTriSplitter.generated.h"

class URetryPrioritizedActorRequestHandlerComponent;
class UItemDataAsset;

UCLASS()
class GSB_API AConveyorTriSplitter : public AConstructibleFacility, public IInputPortHandler, public IOutputPortHandler
{
	GENERATED_BODY()
public:
	AConveyorTriSplitter();

	virtual void RegisterInputPort(AInputPort* InputPort);

	virtual void RegisterOutputPort(AOutputPort* OutputPort);

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

private:
	UFUNCTION()
	bool HandleInputPort(AActor* Actor);

	UFUNCTION()
	bool HandleOutputPort(AActor* Actor);

	UFUNCTION()
	bool CanReceiveItem(const class AInputPort* InputPort);
};
