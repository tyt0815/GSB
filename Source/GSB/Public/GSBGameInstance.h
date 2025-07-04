// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GSBGameInstance.generated.h"

class AFacility;
class AFacilityBuilder;
class AItemCrate;

UCLASS()
class GSB_API UGSBGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// IsValid를 사용하여 Invalid한 객체를 배열에서 삭제한다.
	void CleanFacilityList(TArray<AFacility*>& Facilities);
	void SetFacilitiesPowerInfluenceVisibility(TArray<AFacility*>& Facilities, bool bVisibility);
	void SetPowerInfluenceVisibilityByFacility(AFacility* Facility, bool bVisibility);
	TSubclassOf<AFacilityBuilder> GetDefaultFacilityBuilderClass() const;
	TSubclassOf<AItemCrate> GetDefaultItemCrateClass() const;
	
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "GSBGameInstance|Actor Classes")
	TSubclassOf<AFacilityBuilder> DefaultFacilityBuilderClass;

	UPROPERTY(EditDefaultsOnly, Category = "GSBGameInstance|Actor Classes")
	TSubclassOf<AItemCrate> DefaultItemCrateClass;

private:
	TArray<AFacility*> AllHubs;
	TArray<AFacility*> AllPowerDistributor;

public:
	FORCEINLINE void AddHub(AFacility* Hub)
	{
		AllHubs.Add(Hub);
	}
	FORCEINLINE void AddPowerDistributor(AFacility* PowerDistributor)
	{
		AllPowerDistributor.Add(PowerDistributor);
	}
};
