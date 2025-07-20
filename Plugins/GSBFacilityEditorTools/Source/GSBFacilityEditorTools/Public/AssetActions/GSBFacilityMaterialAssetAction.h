// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetActionUtility.h"
#include "GSBFacilityMaterialAssetAction.generated.h"

/**
 * 
 */
UCLASS()
class GSBFACILITYEDITORTOOLS_API UGSBFacilityMaterialAssetAction : public UAssetActionUtility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(CallInEditor)
	void SetDissolveMaterialFunction();
};
