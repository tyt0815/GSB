// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Facility.generated.h"

class UBoxComponent;



UCLASS()
class GSB_API AFacility : public AActor
{
	GENERATED_BODY()
	
public:	
	AFacility();
	virtual void Tick(float Delta) override;
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

	////////////////////////////////////////////////////////////
	// Grid 기반 시스템
	////////////////////////////////////////////////////////////
public:
	void SnapActorToGrid(const FVector& NewLocation, bool bSweep = false, FHitResult* OutSweepHitResult = nullptr, ETeleportType Teleport = ETeleportType::None);

protected:
	FVector CalcBoxExtentFromGridSize(FVector InGridSize);
	struct FBoxTraceFromGridBoundsParameters
	{
		FVector Start;
		FVector End;
		FVector HalfSize;
		FRotator Orientation;
	};
	void InitializeBoxTraceFromGridBoundsParams(FBoxTraceFromGridBoundsParameters& Params, float Padding = 0);
	void BoxTraceSingleFromGridBoundsForObjects(
		const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes,
		const TArray<AActor*>& ActorsToIgnore,
		FHitResult& HitResult,
		float Padding = 0
	);
	void BoxTraceMultiFromGridBoundsForObjects(
		const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes,
		const TArray<AActor*>& ActorsToIgnore,
		TArray<FHitResult>& HitResults,
		float Padding = 0
	);
	FVector GetSnappedGridLocation(const FVector& Location);

	// 액터가 차지하는 Grid의 범위를 나타내는 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* GridBoundsComponent;

	UPROPERTY(EditAnywhere, Category = "AFacility|Grid Placement")
	FUintVector GridSize = FUintVector(1, 1, 1);

	////////////////////////////////////////////////////////////
	// Get, Set 함수
	////////////////////////////////////////////////////////////
public:
	FORCEINLINE UBoxComponent* GetGridBoundsComponent() const
	{
		return GridBoundsComponent;
	}
};
