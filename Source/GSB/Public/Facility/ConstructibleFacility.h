// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Facility/Facility.h"
#include "ConstructibleFacility.generated.h"

UCLASS()
class GSB_API AConstructibleFacility : public AFacility
{
	GENERATED_BODY()

public:
	AConstructibleFacility();

protected:
	virtual void BeginPlay() override;

public:
	bool IsConstructed() const;

	UFUNCTION(BlueprintPure, Category = "AConstructibleFacility|Constructing")
	float GetConstructionProgress() const;

	UFUNCTION(BlueprintNativeEvent, Category = "AConstructibleFacility|Constructing")
	void BeginConstruction();
	virtual void BeginConstruction_Implementation();

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "AConstructibleFacility|Constructing")
	void CompleteConstruction();
	virtual void CompleteConstruction_Implementation();	

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AConstructibleFacility|Constructing")
	float ConstructionTime = 2;

	FTimerHandle ConstructionTimer;
};
