// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FacilityBuilder.generated.h"

class AConstructibleFacility;
class AHubLinkedFacility;
class APowerDistributorLinkedFacility;

UCLASS()
class GSB_API AFacilityBuilder : public AActor
{
	GENERATED_BODY()
public:
	AFacilityBuilder();
	virtual void Tick(float Delta) override;

public:
	void PreviewFacility(TSubclassOf<AConstructibleFacility> FacilityClass);
	void ConfirmFacilityPlacement();
	void CancelFacilityPreview();
	bool IsPlacingFacility();

protected:
	UPROPERTY(VisibleAnywhere)
	UDecalComponent* DecalComponent;

	UPROPERTY(EditDefaultsOnly, Category = "FacilityBuilder")
	UMaterialInterface* ValidPlacementDecal;
	
	UPROPERTY(EditDefaultsOnly, Category = "FacilityBuilder")
	UMaterialInterface* InvalidPlacementDecal;

private:
	UPROPERTY()
	AConstructibleFacility* FacilityPreview = nullptr;
};
