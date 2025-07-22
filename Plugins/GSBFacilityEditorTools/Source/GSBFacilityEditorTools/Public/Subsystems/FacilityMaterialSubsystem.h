// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "FacilityMaterialSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class GSBFACILITYEDITORTOOLS_API UFacilityMaterialSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

public:
	class UMaterialExpressionStaticBoolParameter* CreateMaterialExpressionStaticBoolParameter(UMaterial* Material);

	class UMaterialExpressionTextureObjectParameter* CreateMaterialExpressionTextureObjectParameter(UMaterial* Material);

	class UMaterialExpressionScalarParameter* CreateMaterialExpressionScalarParameter(UMaterial* Material);

	class UMaterialExpressionVectorParameter* CreateMaterialExpressionVectorParameter(UMaterial* Material);

	void CreateOrUpdateDissolveMaterialFunctionNode(UMaterial* Material);

	bool IsDissolveMaterialFunctionLinkedToEmissiveOrOpacityMask(UMaterial* Material);

	bool IsDissolveMaterialFunctionLinked(UMaterialExpression* MaterialExpression);

	FName GetDissolveMaterialFunctionParameterName(const FFunctionExpressionInput& FunctionExpressionInput) const;

private:
	UMaterialFunctionInterface* DissolveMaterialFunction;
	UTexture* DissolvePatternTexture;

};
