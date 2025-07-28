// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Facility/ConstructibleFacility.h"
#include "Facility/GSBFacilityDataAsset.h"
#include "FacilityBuilder.generated.h"

class AFacilityGhostActor;
class UGSBFacilityQuickSlotList;

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

protected:
	virtual void BeginPlay() override;

public:
	void PreviewFacilityAt(int32 Index);

	void PreviewFacilityByFacilityData(UGSBFacilityDataAsset* FacilityData);

	void ConfirmFacilityPlacement();

	void CancelPreview();

	void RotatePreview();

	void SetFacilityQuickSlot(UGSBFacilityDataAsset* FacilityData, int32 Index);

	void ToggleBuildableFacilityList();

private:
	void PreviewGeneralFacility(UGSBFacilityDataAsset* FacilityData);

	void PreviewMiningFacility();

	void PreviewConveyorBelt();

	AConstructibleFacility* BuildFacility(TSubclassOf<AConstructibleFacility> FacilityClass, const FTransform& Transform);

	void BuildFirstConveyorBelt();

	void Tick_GeneralFacilityBuildMode(float DeltaSeconds);

	void Tick_MiningFacilityBuildMode(float DeltaSeconds);

	void Tick_ConveyorBeltBuildMode(float DeltaSeconds);

	void ConfirmPlacement_GeneralFacilityBuildMode();

	void ConfirmPlacement_MiningFacilityBuildMode();

	void ConfirmPlacement_ConveyorBeltBuildMode();

	bool IsValidGeneralFacilityPlace(AFacilityGhostActor* Ghost);

	bool IsValidMiningFacilityPlace(AFacilityGhostActor* Ghost);

	void SetPowerInfluenceVisibilityByFacilityData(UGSBFacilityDataAsset* FacilityData, bool bVisible);

	void SetPowerInfluenceVisibilityByFacilityClass(UClass* FacilityClass, bool bVisible);

	void SetPowerInfluenceVisibilityByFacility(AFacility* Facility, bool bVisible);

	void SetPowerInfluenceVisibility(bool bVisible);

	AFacilityGhostActor* SpawnFacilityGhost(const TSubclassOf<AConstructibleFacility>& FacilityClass);

	void TraceBuildBlockerInGhostGridBounds(AFacilityGhostActor* Ghost, FHitResult& HitResult);

	void DestroyAllConveyorBeltGhosts();

	TStaticArray<UGSBFacilityDataAsset*, 10> FacilityPreviewQuickSlot;

	UGSBFacilityQuickSlotList* FacilityQuickSlotListWidget;

	UPROPERTY(EditDefaultsOnly, Category = "GSB|FacilityBuilder")
	TArray<UGSBFacilityDataAsset*> GeneralFacilityData;

	UPROPERTY(EditDefaultsOnly, Category = "GSB|FacilityBuilder")
	UGSBFacilityDataAsset* MiningFacilityData;

	UPROPERTY(EditDefaultsOnly, Category = "GSB|FacilityBuilder")
	UGSBFacilityDataAsset* ConveyorBeltForwardData;

	UPROPERTY(EditDefaultsOnly, Category = "GSB|FacilityBuilder")
	UGSBFacilityDataAsset* ConveyorBeltRightData;

	UPROPERTY(EditDefaultsOnly, Category = "GSB|FacilityBuilder")
	UGSBFacilityDataAsset* ConveyorBeltLeftData;

	UGSBFacilityDataAsset* CurrentPreviewFacilityData;

	AFacilityGhostActor* FacilityGhost;

	class UGSBConstructableFacilityListWindow* ConstructableFacilityListWindow;;

	TArray<AFacilityGhostActor*> ConveyorBeltGhosts;

	// 컨베이어 벨트를 지을 때 첫번째 컨베이어 벨트 기준으로 방향을 규정한다.
	enum class EChainRelativeDirection : uint8
	{
		ECRD_Forward,
		ECRD_Backward,
		ECRD_Right,
		ECRD_Left
	};

	void BuildConveyorBeltByChainRelativeDirection(EChainRelativeDirection ChainRelativeDirection, const FTransform& BuildTransform);

	EChainRelativeDirection ChainRelativeDirection_LastTick = EChainRelativeDirection::ECRD_Forward;

	EBuildMode BuildMode = EBuildMode::EBT_None;

public:
	FORCEINLINE const TStaticArray<UGSBFacilityDataAsset*, 10>& GetFacilityPreviewQuickSlot() const
	{
		return FacilityPreviewQuickSlot;
	}
	FORCEINLINE const TArray<UGSBFacilityDataAsset*>& GetGeneralFacilityData() const
	{
		return GeneralFacilityData;
	}
	FORCEINLINE UGSBFacilityDataAsset* GetMiningFacilityData() const
	{
		return MiningFacilityData;
	}
	FORCEINLINE UGSBFacilityDataAsset* GetConveyorBeltForwardData() const
	{
		return ConveyorBeltForwardData;
	}
	FORCEINLINE UGSBFacilityDataAsset* GetConveyorBeltLeftData() const
	{
		return ConveyorBeltLeftData;
	}
	FORCEINLINE UGSBFacilityDataAsset* GetConveyorBeltRightData() const
	{
		return ConveyorBeltRightData;
	}
	FORCEINLINE void SetFacilityQuickSlotListWidget(UGSBFacilityQuickSlotList* Widget)
	{
		FacilityQuickSlotListWidget = Widget;
	}
};
