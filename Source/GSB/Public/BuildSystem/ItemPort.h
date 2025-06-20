// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/ItemTransfer.h"
#include "ItemPort.generated.h"

class UBoxComponent;

UCLASS()
class GSB_API AItemPort : public AActor, public IItemTransfer
{
	GENERATED_BODY()
	
public:	
	AItemPort();
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

	virtual void InitializeSplineSpeed() override;

protected:
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* GridBoundsComponent;

	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* InstancedStaticMeshComponent;

	UPROPERTY(VisibleAnywhere)
	USplineComponent* SplineComponent;

	UPROPERTY(VisibleAnywhere, Category = "AItemPort")
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

	virtual void SetPayload(AItemCrate* ItemCrate) override
	{
		Payload = ItemCrate;
	}
};
