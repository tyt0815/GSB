// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridSnapActor.generated.h"

class UGridSizeBoxComponent;

UCLASS()
class GSB_API AGridSnapActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AGridSnapActor();

public:	
	FUintVector GetGridBoundsSize() const;

	FVector GetGridBoxExtent() const;

	// @return 해당 액터의 천장면의 Location
	FVector GetLocationOnTopXYPlane() const;

	// @return 해당 액터의 바닥면의 Location
	FVector GetLocationOnBottomXYPlane() const;

	void SnapActorToGrid(const FVector& NewLocation, bool bSweep = false, FHitResult* OutSweepHitResult = nullptr, ETeleportType Teleport = ETeleportType::None);

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

	FVector GetSnappedGridLocation(const FVector& Location) const;

	void ResizeGridBounds(const FUintVector GridSize);

protected:
	// 액터가 차지하는 Grid의 범위를 나타내는 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UGridSizeBoxComponent* GridBoundsComponent;

public:
	FORCEINLINE UGridSizeBoxComponent* GetGridBoundsComponent() const
	{
		return GridBoundsComponent;
	}
};
