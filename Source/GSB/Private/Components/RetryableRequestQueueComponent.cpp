// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/RetryableRequestQueueComponent.h"

void URetryableRequestQueueComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ProcessQueuedRequests();
}

void URetryableRequestQueueComponent::EnqueueRequest(AActor* Actor)
{
	ActiveQueue->Enqueue(Actor);
}

void URetryableRequestQueueComponent::ProcessQueuedRequests()
{
	if (!OnProgressRequest.IsBound())
	{
		return;
	}

	AActor* Actor = nullptr;
	while (ActiveQueue->Dequeue(Actor))
	{
		if (!IsValid(Actor))
		{
			continue;
		}
		if (!OnProgressRequest.Execute(Actor))
		{
			RetryQueue->Enqueue(Actor);
		}
	}

	Swap(ActiveQueue, RetryQueue);
}
