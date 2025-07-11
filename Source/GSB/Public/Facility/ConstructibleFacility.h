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
	virtual bool IsOperating() const override;

protected:
	virtual void BeginPlay() override;

public:
	bool IsConstructed() const;

	bool IsConstructing() const;

	bool IsDeconstructing() const;

	UFUNCTION(BlueprintPure, Category = "AConstructibleFacility|Construction")
	float GetConstructionProgress() const;

	UFUNCTION(BlueprintPure, Category = "AConstructibleFacility|Construction")
	float GetDeconstructionProgress() const;

	UFUNCTION(BlueprintNativeEvent, Category = "AConstructibleFacility|Construction")
	void BeginConstruction();
	virtual void BeginConstruction_Implementation();

protected:
	UFUNCTION()
	void HandleDeconstructRequest(AActor* Interactor);

	UFUNCTION(BlueprintNativeEvent, Category = "AConstructibleFacility|Construction")
	void BeginDeconstruction();
	virtual void BeginDeconstruction_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "AConstructibleFacility|Construction")
	void CompleteConstruction();
	virtual void CompleteConstruction_Implementation();	

	UFUNCTION(BlueprintNativeEvent, Category = "AConstructibleFacility|Construction")
	void CompleteDeconstruction();
	virtual void CompleteDeconstruction_Implementation();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AConstructibleFacility|Construction")
	float ConstructionTime = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AConstructibleFacility|Construction")
	float DeconstructionTime = 2;

	FTimerHandle ConstructionTimer;
	FTimerHandle DeconstructionTimer;
};
