// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Facility.h"
#include "Interfaces/PowerProviderFacility.h"
#include "Interfaces/HubFacility.h"
#include "CentralHub.generated.h"

/**
 * 
 */
UCLASS()
class GSB_API ACentralHub : public AFacility, public IPowerProviderFacility, public IHubFacility
{
	GENERATED_BODY()
	
public:
	ACentralHub();

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Tick(float Delta) override;

	virtual bool CanProvidePower() override;

protected:
	virtual void BeginPlay() override;

	///////////////////////////////////////////////////////
	// Power 공급
	///////////////////////////////////////////////////////
public:

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* PowerInfluenceAreaComponent;

	UPROPERTY(EditAnywhere, Category = "ACentralHub")
	uint32 PowerInfluenceAreaRadiusInGridUnits;

private:
	int PowerCapacity = 500;

public:
	FORCEINLINE virtual UStaticMeshComponent* GetPowerInfluenceAreaComponent() const override
	{
		return PowerInfluenceAreaComponent;
	}
	FORCEINLINE virtual uint32 GetPowerInfluenceAreaRadiusInGridUnits() const override
	{
		return PowerInfluenceAreaRadiusInGridUnits;
	}
};
