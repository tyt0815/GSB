// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SplineTransportComponent.h"

USplineTransportComponent::USplineTransportComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetTickableWhenPaused(false);
}


void USplineTransportComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsActive())
	{
		return;
	}

	TArray<AActor*> ActorsToRemove;
	for (auto& ActiveTransport : ActiveTransports)
	{
		AActor* TransportedActor = ActiveTransport.Key;
		FTransportState* TransportState = &ActiveTransport.Value;
		if (IsValid(TransportedActor))
		{
			TransportState->CurrentDistance += TransportState->TransportSpeed * DeltaTime;
			FTransform NewTransform = GetTransformAtDistanceAlongSpline(TransportState->CurrentDistance, ESplineCoordinateSpace::World);
			TransportedActor->SetActorLocation(NewTransform.GetLocation());
			TransportedActor->SetActorRotation(NewTransform.GetRotation());

			if (TransportState->CurrentDistance >= GetSplineLength())
			{
				OnArrived.Broadcast(TransportedActor);
				ActorsToRemove.Add(TransportedActor);
			}
		}
		else
		{
			ActorsToRemove.Add(TransportedActor);
		}
	}

	for (AActor* ActorToRemove : ActorsToRemove)
	{
		ActiveTransports.Remove(ActorToRemove);
	}
}

void USplineTransportComponent::TransportActor(AActor* Actor, float TransportSpeed)
{
	if (IsValid(Actor) && TransportSpeed != 0)
	{
		FTransportState TransportState;
		TransportState.TransportSpeed = TransportSpeed;
		TransportState.CurrentDistance = TransportSpeed > 0 ? 0 : GetSplineLength();
		ActiveTransports.Add(Actor, TransportState);
	}
}

FVector USplineTransportComponent::GetStartDirection(ESplineCoordinateSpace::Type CoordinateSpace)
{
	return GetDirectionAtSplinePoint(0, CoordinateSpace);
}

FVector USplineTransportComponent::GetEndDirection(ESplineCoordinateSpace::Type CoordinateSpace)
{
	return GetDirectionAtSplinePoint(GetNumberOfSplinePoints() - 1, CoordinateSpace);
}

FVector USplineTransportComponent::GetStartLocation(ESplineCoordinateSpace::Type CoordinateSpace)
{
	return GetLocationAtSplinePoint(0, CoordinateSpace);
}

FVector USplineTransportComponent::GetEndLocation(ESplineCoordinateSpace::Type CoordinateSpace)
{
	return GetLocationAtSplinePoint(GetNumberOfSplinePoints() - 1, CoordinateSpace);
}

bool USplineTransportComponent::IsTransporting() const
{
	return IsActive() && ActiveTransports.Num() > 0;
}

