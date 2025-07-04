// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "SplineTransportComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FComponentOnArrivedSignature, AActor*, Actor);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GSB_API USplineTransportComponent : public USplineComponent
{
	GENERATED_BODY()

public:	
	USplineTransportComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:	
	void TransportActor(AActor* Actor, float TransportSpeed);

	FVector GetStartDirection(ESplineCoordinateSpace::Type CoordinateSpace = ESplineCoordinateSpace::World);

	FVector GetEndDirection(ESplineCoordinateSpace::Type CoordinateSpace = ESplineCoordinateSpace::World);

	FVector GetStartLocation(ESplineCoordinateSpace::Type CoordinateSpace = ESplineCoordinateSpace::World);

	FVector GetEndLocation(ESplineCoordinateSpace::Type CoordinateSpace = ESplineCoordinateSpace::World);

	// 수송중인 액터가 하나라도 있을 경우 true
	bool IsActive() const;

	FComponentOnArrivedSignature OnArrived;

private:
	struct FTransportState
	{
		float CurrentDistance;
		float TransportSpeed;
	};

	TMap<AActor*, FTransportState> ActiveTransports;
};
