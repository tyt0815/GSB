// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/RetryPrioritizedActorRequestHandlerComponent.h"

URetryPrioritizedActorRequestHandlerComponent::URetryPrioritizedActorRequestHandlerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URetryPrioritizedActorRequestHandlerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ProcessRequest();
}

void URetryPrioritizedActorRequestHandlerComponent::RegisterActor(AActor* Actor)
{
	RequestQueue.Enqueue(Actor);
}

void URetryPrioritizedActorRequestHandlerComponent::ProcessRequest()
{
	if (!OnProcess.IsBound())
	{
		return;
	}

	AActor* Actor;
	TArray<AActor*> SuccessedActors;
	TArray<AActor*> FailedActors;
	while (RequestQueue.Dequeue(Actor))
	{
		if (!IsValid(Actor))
		{
			continue;
		}

		if (OnProcess.Execute(Actor))
		{
			SuccessedActors.Add(Actor);
		}
		else
		{
			FailedActors.Add(Actor);
		}
	}

	for (AActor* FailedActor : FailedActors)
	{
		RequestQueue.Enqueue(FailedActor);
	}
	for (AActor* SuccessedActor : SuccessedActors)
	{
		RequestQueue.Enqueue(SuccessedActor);
	}
}

