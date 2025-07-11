// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildSystem/GridSnapActor.h"
#include "FacilityAddon.generated.h"

class UBoxComponent;
class AFacility;

UCLASS()
class GSB_API AFacilityAddon : public AGridSnapActor
{
	GENERATED_BODY()
	
public:	
	AFacilityAddon();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintNativeEvent, Category = "AFacilityAddon|Constructing")
	void CompleteConstruction();
	virtual void CompleteConstruction_Implementation() {}

	virtual void OnConnectedToFacility() {}

	virtual void OnFacilityLinkedToPowerProvider() {}
};
