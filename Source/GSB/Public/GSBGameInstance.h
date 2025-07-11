// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GSBGameInstance.generated.h"

class AFacility;
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

	TSubclassOf<AActor> GetActorClass(const FName& Name) const;

	TSubclassOf<UUserWidget> GetUserWidgetClass(const FName& Name) const;

	UMaterialInterface* GetMaterialInterface(const FName& Name) const;

private:
	TArray<AFacility*> AllHubs;
	TArray<AFacility*> AllPowerDistributor;

	////////////////////////////////////////////////////////////////////////////////
	// Subclass
	////////////////////////////////////////////////////////////////////////////////
	UPROPERTY(EditDefaultsOnly)
	TMap<FName, TSubclassOf<AActor>> ActorClasses;

	UPROPERTY(EditDefaultsOnly)
	TMap<FName, TSubclassOf<UUserWidget>> UserWidgetClasses;

	UPROPERTY(EditDefaultsOnly)
	TMap<FName, UMaterialInterface*> MaterialInterfaces;

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