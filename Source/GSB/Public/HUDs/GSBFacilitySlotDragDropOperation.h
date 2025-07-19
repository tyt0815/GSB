// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "GSBFacilitySlotDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class GSB_API UGSBFacilitySlotDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	class UGSBFacilityDataAsset* FacilityData;
};
