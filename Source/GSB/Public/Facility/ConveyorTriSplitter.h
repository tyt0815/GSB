// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Facility/ConstructibleFacility.h"
#include "Interfaces/InputPortHandler.h"
#include "Interfaces/OutputPortHandler.h"
#include "ConveyorTriSplitter.generated.h"

class URetryPrioritizedActorRequestHandlerComponent;
class UItemStorageComponent;

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

	UPROPERTY(VisibleAnywhere)
	UItemStorageComponent* StorageComponent;

private:
	UFUNCTION()
	bool HandleInputPort(AActor* Actor);

	UFUNCTION()
	bool HandleOutputPort(AActor* Actor);
};
