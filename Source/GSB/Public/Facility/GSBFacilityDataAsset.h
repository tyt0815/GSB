// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Facility/Facility.h"
#include "GSBFacilityDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class GSB_API UGSBFacilityDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	FText FacilityName;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AFacility> FacilityClass;
};
