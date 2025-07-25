// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDataAsset.h"
#include "ProductionRecipeSetDataAsset.generated.h"

USTRUCT()
struct FProductionRecipe
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TMap<UItemDataAsset*, int32> RequiredItems;

	UPROPERTY(EditDefaultsOnly)
	FItemStack ResultItem;

	UPROPERTY(EditDefaultsOnly)
	float ProductionTime;
};



UCLASS()
class GSB_API UProductionRecipeSetDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FProductionRecipe> Recipes;
};
