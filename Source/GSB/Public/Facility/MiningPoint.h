// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildSystem/GridSnapActor.h"
#include "MiningPoint.generated.h"

class UItemDataAsset;
class UInstancedStaticMeshComponent;

UCLASS()
class GSB_API AMiningPoint : public AGridSnapActor
{
	GENERATED_BODY()
public:
	AMiningPoint();

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* InstancedStaticMeshComponent;
	
	UPROPERTY(EditAnywhere, Category = "AMiningPoint")
	FTransform RandomTransformMin;

	UPROPERTY(EditAnywhere, Category = "AMiningPoint")
	FTransform RandomTransformMax;

	UPROPERTY(EditAnywhere, Category = "AMiningPoint")
	int RandomDrawNum = 1;

	UPROPERTY(EditAnywhere, Category = "AMiningPoint")
	int RandomDrawSeed = 0;

	UPROPERTY(EditAnywhere, Category = "AMiningPoint")
	UItemDataAsset* MineableItem;

	UPROPERTY(EditAnywhere, Category = "AMiningPoint", meta = (ClampMin = "1"))
	float MiningTime = 1.0f;


public:
	FORCEINLINE UItemDataAsset* GetMineableItem() const
	{
		return MineableItem;
	}
	FORCEINLINE float GetMiningTime() const
	{
		return MiningTime;
	}
};
