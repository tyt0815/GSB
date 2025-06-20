// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/ConstructibleFacility.h"
#include "Interfaces/ItemTransfer.h"
#include "ConveyorBelt.generated.h"

class USplineComponent;


UCLASS()
class GSB_API AConveyorBelt : public AConstructibleFacility, public IItemTransfer
{
	GENERATED_BODY()
	
public:
	AConveyorBelt();

	virtual void BeginPlay() override;

	virtual void BeginDestroy() override;

	virtual void OnEnterWaitingState_Implementation() override;

	virtual void OnEnterRunningState_Implementation() override;

	// PayLoad가 있을때 Running 상태로 전이
	virtual void Tick_Running_Implementation(float Delta) override;

	virtual void InitializeSplineSpeed() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USplineComponent* SplineComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, Category = "AConveyorBelt")
	AItemCrate* Payload;

public:
	FORCEINLINE virtual USplineComponent* GetSplineComponent() const override
	{
		return SplineComponent;
	}
	FORCEINLINE virtual AItemCrate* GetPayload() const override
	{
		return Payload;
	}
	FORCEINLINE virtual void SetPayload(AItemCrate* ItemCrate) override
	{
		Payload = ItemCrate;
	}
};
