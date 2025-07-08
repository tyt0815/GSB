// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Facility/ConstructibleFacility.h"
#include "FacilityBuilder.generated.h"

class AFacilityGhostActor;

enum class EBuildMode : uint8
{
	EBT_None,
	EBT_GeneralFacility,
	EBT_MiningFacility,
	EBT_ConveyorBelt
};

UCLASS()
class GSB_API AFacilityBuilder : public AActor
{
	GENERATED_BODY()
public:
	AFacilityBuilder();

	virtual void Tick(float DeltaSeconds) override;

public:
	void PreviewGeneralFacility(const FName& FacilityName);

	void PreviewMiningFacility();

	void PreviewConveyorBelt();

	void ConfirmFacilityPlacement();

	void CancelPreview();

	void RotatePreview();

private:
	AConstructibleFacility* BuildFacility(TSubclassOf<AConstructibleFacility> FacilityClass, const FTransform& Transform);

	void Tick_GeneralFacilityBuildMode(float DeltaSeconds);

	void Tick_MiningFacilityBuildMode(float DeltaSeconds);

	void Tick_ConveyorBeltBuildMode(float DeltaSeconds);

	void ConfirmPlacement_GeneralFacilityBuildMode();

	void ConfirmPlacement_MiningFacilityBuildMode();

	void ConfirmPlacement_ConveyorBeltBuildMode();

	bool IsValidGeneralFacilityPlace(AFacilityGhostActor* Ghost);

	bool IsValidMiningFacilityPlace(AFacilityGhostActor* Ghost);

	AFacilityGhostActor* SpawnFacilityGhost(const TSubclassOf<AConstructibleFacility>& FacilityClass, bool bVisiblePowerInfluenceArea);

	void TraceGridBoundsInGhostGridBounds(AFacilityGhostActor* Ghost, FHitResult& HitResult);

	void DestroyAllConveyorBeltGhosts();

	UPROPERTY(EditDefaultsOnly, Category = "AFacilityBuilder")
	TMap<FName, TSubclassOf<AConstructibleFacility>> GeneralFacilityClasses;

	UPROPERTY(EditDefaultsOnly, Category = "AFacilityBuilder")
	TSubclassOf<AConstructibleFacility> MiningFacilityClass;

	UPROPERTY(EditDefaultsOnly, Category = "AFacilityBuilder")
	TSubclassOf<AConstructibleFacility> ConveyorBeltForwardClass;

	UPROPERTY(EditDefaultsOnly, Category = "AFacilityBuilder")
	TSubclassOf<AConstructibleFacility> ConveyorBeltRightClass;

	UPROPERTY(EditDefaultsOnly, Category = "AFacilityBuilder")
	TSubclassOf<AConstructibleFacility> ConveyorBeltLeftClass;

	TSubclassOf<AConstructibleFacility> CurrentGeneralFacilityClass;

	AFacilityGhostActor* FacilityGhost;

	TArray<AFacilityGhostActor*> ConveyorBeltGhosts;

	// 컨베이어 벨트를 지을 때 첫번째 컨베이어 벨트 기준으로 방향을 규정한다.
	enum class EChainBuildDirection : uint8
	{
		ECBD_Forward,
		ECBD_Backward,
		ECBD_Right,
		ECBD_Left
	};
	EChainBuildDirection ChainBuildDirection_LastTick = EChainBuildDirection::ECBD_Forward;

	EBuildMode BuildMode = EBuildMode::EBT_None;
};
