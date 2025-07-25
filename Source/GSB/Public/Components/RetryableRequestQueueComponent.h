// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RetryableRequestQueueComponent.generated.h"

DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(bool, FComponentOnProcessQueuedActorSignature, AActor*, Actor);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GSB_API URetryableRequestQueueComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FComponentOnProcessQueuedActorSignature OnProgressRequest;

	void EnqueueRequest(AActor* Actor);
	
	void ProcessQueuedRequests();

private:
	TQueue<AActor*> ProcessingQueues[2];
	TQueue<AActor*>* ActiveQueue = &ProcessingQueues[0];
	TQueue<AActor*>* RetryQueue = &ProcessingQueues[1];
};
