// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildSystem/GridSnapActor.h"
#include "FacilityGhostActor.generated.h"

class UGhostPreviewComponent;
class UDecalComponent;
class AFacility;

UCLASS()
class GSB_API AFacilityGhostActor : public AGridSnapActor
{
	GENERATED_BODY()
	
public:	
	AFacilityGhostActor();

public:
	void BuildGhostPreview(AFacility* Facility);

	void UpdatePlacementDecal(bool bValid);

protected:
	UPROPERTY(VisibleAnywhere)
	UGhostPreviewComponent* GhostPreviewComponent;

	UPROPERTY(VisibleAnywhere)
	UDecalComponent* PlacementDecal;
};
