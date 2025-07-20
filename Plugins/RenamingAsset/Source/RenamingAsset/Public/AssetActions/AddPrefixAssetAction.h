// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetActionUtility.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "Blueprint/UserWidget.h"
#include "NiagaraSystem.h"
#include "NiagaraEmitter.h"	
#include "AddPrefixAssetAction.generated.h"

/**
 * 
 */
UCLASS()
class RENAMINGASSET_API UAddPrefixAssetAction : public UAssetActionUtility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(CallInEditor)
	void AddPrefix();

	FString GetPrefixByObject(UObject* Object);

	FString	GetPrefix(UClass* Class);

	TMap<UObject*, FString> PrefixMap =
	{
		{UBlueprint::StaticClass(),TEXT("BP_")},
		{UStaticMesh::StaticClass(),TEXT("SM_")},
		{UMaterial::StaticClass(), TEXT("M_")},
		{UMaterialInstanceConstant::StaticClass(),TEXT("MI_")},
		{UMaterialFunctionInterface::StaticClass(), TEXT("MF_")},
		{UParticleSystem::StaticClass(), TEXT("PS_")},
		{USoundCue::StaticClass(), TEXT("SC_")},
		{USoundWave::StaticClass(), TEXT("SW_")},
		{UTexture::StaticClass(), TEXT("T_")},
		{UTexture2D::StaticClass(), TEXT("T_")},
		{UUserWidget::StaticClass(), TEXT("WBP_")},
		{USkeletalMeshComponent::StaticClass(), TEXT("SK_")},
		{UNiagaraSystem::StaticClass(), TEXT("NS_")},
		{UNiagaraEmitter::StaticClass(), TEXT("NE_")}
	};
};
