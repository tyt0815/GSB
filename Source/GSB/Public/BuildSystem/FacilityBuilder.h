// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildSystem/GridSnapActor.h"
#include "FacilityBuilder.generated.h"

class UGhostPreviewComponent;
class AFacility;
class AConstructibleFacility;
class AHubLinkedFacility;
class APowerDistributorLinkedFacility;

namespace EBuildCategory
{
	enum Type : uint8
	{
		EBC_GeneralFacility,
		EBC_ConveyorBelt,
		EBC_MiningFacility
	};
}

UCLASS()
class GSB_API AFacilityBuilder : public AGridSnapActor
{
	GENERATED_BODY()
public:
	AFacilityBuilder();
	virtual void Tick(float Delta) override;

public:
	void PreviewFacility(TSubclassOf<AConstructibleFacility> FacilityClass);
	void ConfirmFacilityPlacement();
	void CancelFacilityPreview();
	bool IsPlacingFacility() const;
	bool IsValidPlace() const;

protected:
	UPROPERTY(VisibleAnywhere)
	UDecalComponent* DecalComponent;

	UPROPERTY(VisibleAnywhere)
	UGhostPreviewComponent* GhostPreviewComponent;

	UPROPERTY(EditDefaultsOnly, Category = "FacilityBuilder")
	UMaterialInterface* ValidPlacementDecal;
	
	UPROPERTY(EditDefaultsOnly, Category = "FacilityBuilder")
	UMaterialInterface* InvalidPlacementDecal;

private:
	TSubclassOf<AConstructibleFacility> PreviewFacilityClass = nullptr;

	void SetPowerInfluenceVisibilityByFacility(AFacility* Facility, bool bVisibility);

	void SetPowerInfluenceVisibilityByFacilityClass(TSubclassOf<AConstructibleFacility> FacilityClass, bool bVisibility);

	EBuildCategory::Type BuildCategory = EBuildCategory::EBC_GeneralFacility;
};
