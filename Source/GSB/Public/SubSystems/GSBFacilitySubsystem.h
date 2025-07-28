// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/HubFacility.h"
#include "Interfaces/PowerProviderFacility.h"
#include "GSBFacilitySubsystem.generated.h"

class AFacility;
class APowerDistributor;
class ACentralHub;

UCLASS()
class GSB_API UGSBFacilitySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	static UGSBFacilitySubsystem* Get(const UObject* WorldContext);

	void SetFacilityPowerInfluenceVisibility(TScriptInterface<IPowerProviderFacility> PowerProvider, bool bVisibility);

	void SetAllHubsPowerInfluenceVisibility(bool bVisibility);

	void SetAllPowerDistributorsPowerInfluenceVisibility(bool bVisibility);

	void SetAllPowerInfluenceVisibility(bool bVisibility);

	void SetPowerInfluenceVisibilityByFacility(AFacility* Facility, bool bVisibility);

	void SetPowerInfluenceVisibility(bool bVisibility);

	void SetCentralHub(ACentralHub* InCentralHub);

private:
	void CleanInvalidHubsInArray();

	void CleanInvalidPowerDistributorsInArray();

	ACentralHub* CentralHub;

	TArray<TScriptInterface<IHubFacility>> AllHubs;
	TArray<APowerDistributor*> AllPowerDistributor;

public:
	FORCEINLINE void AddHub(TScriptInterface<IHubFacility> Hub)
	{
		AllHubs.Add(Hub);
	}
	FORCEINLINE void AddPowerDistributor(APowerDistributor* PowerDistributor)
	{
		AllPowerDistributor.Add(PowerDistributor);
	}
	FORCEINLINE ACentralHub* GetCentralHub() const
	{
		return CentralHub;
	}
};
