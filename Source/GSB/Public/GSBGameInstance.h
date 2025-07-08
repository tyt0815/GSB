// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Items/ItemCrate.h"
#include "Items/DroppedItem.h"
#include "BuildSystem/FacilityBuilder.h"
#include "GSBGameInstance.generated.h"

class AFacility;
class AItemCrate;
struct FItemStack;

UCLASS()
class GSB_API UGSBGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// IsValid를 사용하여 Invalid한 객체를 배열에서 삭제한다.
	void CleanFacilityList(TArray<AFacility*>& Facilities);

	void SetFacilitiesPowerInfluenceVisibility(TArray<AFacility*>& Facilities, bool bVisibility);

	void SetPowerInfluenceVisibilityByFacility(AFacility* Facility, bool bVisibility);

	void SetPowerInfluenceVisibility(bool bVisibility);

	TSubclassOf<AItemCrate> GetDefaultItemCrateClass() const;
	
	ADroppedItem* SpawnDroppedItem(const FItemStack& ItemStack);

	AFacilityBuilder* SpawnFacilityBuilder();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "UGSBGameInstance|Actor Classes")
	TSubclassOf<AItemCrate> DefaultItemCrateClass;

	UPROPERTY(EditDefaultsOnly, Category = "UGSBGameInstance|Actor Classes")
	TSubclassOf<ADroppedItem> DefaultDroppedItemClass;

	UPROPERTY(EditDefaultsOnly, Category = "UGSBGameInstance|Actor Classes")
	TSubclassOf<AFacilityBuilder> DefaultFacilityBuilderClass;

	UPROPERTY(EditDefaultsOnly, Category = "UGSBGameInstance|Materials")
	UMaterialInterface* ValidPlacementDecal;

	UPROPERTY(EditDefaultsOnly, Category = "UGSBGameInstance|Materials")
	UMaterialInterface* InvalidPlacementDecal;

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
	FORCEINLINE UMaterialInterface* GetValidPlacementDecal() const
	{
		return ValidPlacementDecal;
	}
	FORCEINLINE UMaterialInterface* GetInvalidPlacementDecal() const
	{
		return InvalidPlacementDecal;
	}
};
