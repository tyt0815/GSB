// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GridSizeBoxComponent.generated.h"

/**
 * 
 */
UCLASS()
class GSB_API UGridSizeBoxComponent : public UBoxComponent
{
	GENERATED_BODY()
public:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

public:
	FVector CalcBoxExtentFromGridSize(FVector InGridSize) const;

	struct FBoxTraceFromGridBoundsParameters
	{
		FVector Start;
		FVector End;
		FVector HalfSize;
		FRotator Orientation;
	};

	void InitializeBoxTraceFromGridBoundsParams(FBoxTraceFromGridBoundsParameters& Params, float Padding = 0) const;

	void BoxTraceSingleFromGridBoundsForObjects(
		const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes,
		const TArray<AActor*>& ActorsToIgnore,
		FHitResult& HitResult,
		float Padding = 0
	) const;

	void BoxTraceMultiFromGridBoundsForObjects(
		const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes,
		const TArray<AActor*>& ActorsToIgnore,
		TArray<FHitResult>& HitResults,
		float Padding = 0
	) const;

	void InitializeBoxTraceForwardMultiByYZExtentParams(FBoxTraceFromGridBoundsParameters& Params, float Distance, float Padding = 0) const;

	void BoxTraceForwardMultiByYZExtentForObjects(
		const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes,
		const TArray<AActor*>& ActorsToIgnore,
		TArray<FHitResult>& HitResults,
		float Distance,
		float Padding = 0
	);

	void ResizeGridBounds(FUintVector InGridSize);

protected:
	UPROPERTY(EditAnywhere, Category = "UGridSizeBoxComponent")
	FUintVector GridSize = FUintVector(0, 0, 0);

public:
	FORCEINLINE	FUintVector GetGridSize() const
	{
		return GridSize;
	}
};
