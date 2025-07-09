// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Facility/PowerDistributorLinkedFacility.h"
#include "Interfaces/OutputPortHandler.h"
#include "MiningFacility.generated.h"

class UItemStorageComponent;
class AMiningPoint;
class AOutputPort;

UCLASS()
class GSB_API AMiningFacility : public APowerDistributorLinkedFacility, public IOutputPortHandler
{
	GENERATED_BODY()
	
public:
	AMiningFacility();

	virtual void Tick(float DeltaSeconds) override;

	virtual void OnLinkToPowerProvider_Implementation() override;

	virtual void OnUnlinkFromPowerProvider_Implementation() override;

	virtual void RegisterOutputPort(AOutputPort* OutputPort) override;


protected:
	virtual void BeginPlay() override;

private:
	bool TryOccupyMiningPoint();

	AOutputPort* ConnectedOutputPort;

	UPROPERTY(VisibleAnywhere)
	AMiningPoint* OccupiedMiningPoint;

	float CurrentMiningTime = 0;
};
