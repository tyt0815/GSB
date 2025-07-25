// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildSystem/GridSnapActor.h"
#include "Components/GridSizeBoxComponent.h"
#include "GSBDefines.h"

AGridSnapActor::AGridSnapActor()
{
	PrimaryActorTick.bCanEverTick = false;

	GridBoundsComponent = CreateDefaultSubobject<UGridSizeBoxComponent>(TEXT("GridBounds"));
	SetRootComponent(GridBoundsComponent);
	GridBoundsComponent->SetCollisionProfileName(TEXT("GridBounds"));
}

FUintVector AGridSnapActor::GetGridBoundsSize() const
{
	return GridBoundsComponent->GetGridSize();
}

FVector AGridSnapActor::GetGridBoxExtent() const
{
	return GridBoundsComponent->GetScaledBoxExtent();
}

FVector AGridSnapActor::GetLocationOnTopXYPlane() const
{
	return GetActorLocation() + FVector::ZAxisVector * GetGridBoxExtent().Z;
}

FVector AGridSnapActor::GetLocationOnBottomXYPlane() const
{
	return GetActorLocation() - FVector::ZAxisVector * GetGridBoxExtent().Z;
}

void AGridSnapActor::SnapActorToGrid(const FVector& NewLocation, bool bSweep, FHitResult* OutSweepHitResult, ETeleportType Teleport)
{
	SetActorLocation(GetSnappedGridLocation(NewLocation), bSweep, OutSweepHitResult, Teleport);
}

void AGridSnapActor::BoxTraceSingleFromGridBoundsForObjects(const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const TArray<AActor*>& ActorsToIgnore, FHitResult& HitResult, float Padding) const
{
	GridBoundsComponent->BoxTraceSingleFromGridBoundsForObjects(ObjectTypes, ActorsToIgnore, HitResult, Padding);
}

void AGridSnapActor::BoxTraceMultiFromGridBoundsForObjects(const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const TArray<AActor*>& ActorsToIgnore, TArray<FHitResult>& HitResults, float Padding) const
{
	GridBoundsComponent->BoxTraceMultiFromGridBoundsForObjects(ObjectTypes, ActorsToIgnore, HitResults, Padding);
}

FVector AGridSnapActor::GetSnappedGridLocation(const FVector& Location) const
{
	FVector GridLocation = Location;
	int32 HalfSize = GRID_CELL_SIZE / 2;
	GridLocation.X = GridLocation.X > 0 ? GridLocation.X + HalfSize : GridLocation.X - HalfSize;
	GridLocation.X -= static_cast<int64>(GridLocation.X) % GRID_CELL_SIZE;
	FMath::Modf(GridLocation.X, &GridLocation.X);
	GridLocation.Y = GridLocation.Y > 0 ? GridLocation.Y + HalfSize : GridLocation.Y - HalfSize;
	GridLocation.Y -= static_cast<int64>(GridLocation.Y) % GRID_CELL_SIZE;
	FMath::Modf(GridLocation.Y, &GridLocation.Y);
	GridLocation.Z += GridBoundsComponent->GetScaledBoxExtent().Z;
	return GridLocation;
}

void AGridSnapActor::ResizeGridBounds(const FUintVector GridSize)
{
	GridBoundsComponent->ResizeGridBounds(GridSize);
}