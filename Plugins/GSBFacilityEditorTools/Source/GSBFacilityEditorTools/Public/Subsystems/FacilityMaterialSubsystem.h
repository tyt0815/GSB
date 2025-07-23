// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "MaterialEditingLibrary.h"
#include "FacilityMaterialSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class GSBFACILITYEDITORTOOLS_API UFacilityMaterialSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()
public:
	struct FDissolveMaterialFunctionParameters
	{
		bool bCreateMaterialInstance;
		bool bHueShift;
		bool bSwitchUVs;
		bool bUseOnlyTexture;
		float Amount;
		float Tiling;
		float Width;
		FLinearColor FringeColor;
		UTexture* Pattern;
	};

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

public:
	UMaterialInstanceConstant* CreateMaterialInstanceConstant(const FString& NameOfTheMaterialInst, const FString PackagePath);

	UMaterialInstanceConstant* FindOrCreateMaterialInstanceConstant(const FString& NameOfTheMaterialInst, const FString PackagePath);

	template<typename MaterialExpressionT>
	MaterialExpressionT* CreateMaterialExpression(UMaterial* Material);

	UMaterialExpression* FindMaterialExpressionByDesc(UMaterial* Material, const FString& Desc);

	class UMaterialExpressionParameter* FindMaterialExpressionParameterByName(UMaterial* Material, const FName& ParameterName);

	class UMaterialExpressionTextureSampleParameter* FindMaterialExpressionTextureSampleParameterByName(UMaterial* Material, const FString& ParameterName);

	void CreateOrUpdateDissolveMaterialFunctionNode(UMaterial* Material, const FDissolveMaterialFunctionParameters& Params);

	FAssetData FindOrCreateAssetWithSuffix(const FAssetData& AssetData, FString Suffix);

	FAssetData CreateOrUpdateDissolveMaterialAsset(const FAssetData& MaterialAssetData, const FDissolveMaterialFunctionParameters& Params);

	void CreateOrUpdateDissolveMaterialAssets(TConstArrayView<TSharedPtr<FAssetData>> MaterialAssetDatas, const FDissolveMaterialFunctionParameters& Params);

	bool IsDissolveMaterialFunctionLinkedToEmissiveOrOpacityMask(UMaterial* Material);

	bool IsDissolveMaterialFunctionLinked(UMaterialExpression* MaterialExpression);

	FName GetDissolveMaterialFunctionParameterName(const FFunctionExpressionInput& FunctionExpressionInput) const;

private:
	UMaterialFunctionInterface* DissolveMaterialFunction;
	UTexture* DissolvePatternTexture;

};

template<typename MaterialExpressionT>
inline MaterialExpressionT* UFacilityMaterialSubsystem::CreateMaterialExpression(UMaterial* Material)
{
	return Cast<MaterialExpressionT>(UMaterialEditingLibrary::CreateMaterialExpression(Material, MaterialExpressionT::StaticClass()));
}