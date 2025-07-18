// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/GridSizeBoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GSBDefines.h"

void UGridSizeBoxComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	ResizeGridBounds(GridSize);
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

FVector UGridSizeBoxComponent::CalcBoxExtentFromGridSize(FVector InGridSize) const
{
	FVector BoxExtentInGrid(GRID_CELL_SIZE / 2);
	BoxExtentInGrid += FVector(InGridSize) * GRID_CELL_SIZE;
	return BoxExtentInGrid;
}

void UGridSizeBoxComponent::InitializeBoxTraceFromGridBoundsParams(FBoxTraceFromGridBoundsParameters& Params, float Padding) const
{
	FVector Up = GetUpVector();
	Params.Start = GetComponentLocation() + Up * (GetScaledBoxExtent().Z - Padding);
	Params.End = GetComponentLocation() - Up * (GetScaledBoxExtent().Z - Padding);
	Params.HalfSize.X = GetScaledBoxExtent().X - Padding;
	Params.HalfSize.Y = GetScaledBoxExtent().Y - Padding;
	Params.HalfSize.Z = 0.0f;
	Params.Orientation = GetComponentRotation();
}

void UGridSizeBoxComponent::BoxTraceSingleFromGridBoundsForObjects(const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const TArray<AActor*>& ActorsToIgnore, FHitResult& HitResult, float Padding) const
{
	FBoxTraceFromGridBoundsParameters Params;
	InitializeBoxTraceFromGridBoundsParams(Params, Padding);

	UKismetSystemLibrary::BoxTraceSingleForObjects(
		this,
		Params.Start,
		Params.End,
		Params.HalfSize,
		Params.Orientation,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResult,
		true
	);
}

void UGridSizeBoxComponent::BoxTraceMultiFromGridBoundsForObjects(const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const TArray<AActor*>& ActorsToIgnore, TArray<FHitResult>& HitResults, float Padding) const
{
	FBoxTraceFromGridBoundsParameters Params;
	InitializeBoxTraceFromGridBoundsParams(Params, Padding);

	UKismetSystemLibrary::BoxTraceMultiForObjects(
		this,
		Params.Start,
		Params.End,
		Params.HalfSize,
		Params.Orientation,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResults,
		true
	);
}

void UGridSizeBoxComponent::InitializeBoxTraceForwardMultiByYZExtentParams(FBoxTraceFromGridBoundsParameters& Params, float Distance, float Padding) const
{
	FVector Foward = GetForwardVector();
	Params.Start = GetComponentLocation();
	Params.End = GetComponentLocation() + Foward * Distance;
	Params.HalfSize.X = 0;
	Params.HalfSize.Y = GetScaledBoxExtent().Y - Padding;
	Params.HalfSize.Z = GetScaledBoxExtent().Z - Padding;
	Params.Orientation = GetComponentRotation();
}

void UGridSizeBoxComponent::BoxTraceForwardMultiByYZExtentForObjects(
	const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes,
	const TArray<AActor*>& ActorsToIgnore,
	TArray<FHitResult>& HitResults,
	float Distance,
	float Padding
)
{
	FBoxTraceFromGridBoundsParameters Params;
	InitializeBoxTraceForwardMultiByYZExtentParams(Params, Distance, Padding);

	UKismetSystemLibrary::BoxTraceMultiForObjects(
		this,
		Params.Start,
		Params.End,
		Params.HalfSize,
		Params.Orientation,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		HitResults,
		true
	);
}

void UGridSizeBoxComponent::ResizeGridBounds(FUintVector InGridSize)
{
	GridSize = InGridSize;
	SetBoxExtent(CalcBoxExtentFromGridSize(FVector(GridSize)));
}
