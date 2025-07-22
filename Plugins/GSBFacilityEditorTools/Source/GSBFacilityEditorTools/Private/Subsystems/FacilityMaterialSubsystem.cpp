// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/FacilityMaterialSubsystem.h"
#include "Materials/MaterialExpressionFunctionInput.h"
#include "Materials/MaterialExpressionScalarParameter.h"
#include "Materials/MaterialExpressionStaticBoolParameter.h"
#include "Materials/MaterialExpressionVectorParameter.h"
#include "Materials/MaterialExpressionTextureObjectParameter.h"
#include "MaterialEditingLibrary.h"
#include "GSBDebugLibrary.h"

void UFacilityMaterialSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	DissolveMaterialFunction = LoadObject<UMaterialFunctionInterface>(this, TEXT("/Script/Engine.MaterialFunction'/GSBFacilityEditorTools/MF_DissolveEffect.MF_DissolveEffect'"));
	if (!IsValid(DissolveMaterialFunction))
	{
		TRACE_PRINT_SCREEN_AND_LOG(TEXT("DissolveMaterialFunction가 invalid합니다."));
	}

	DissolvePatternTexture = LoadObject<UTexture>(this, TEXT("/Script/Engine.Texture2D'/GSBFacilityEditorTools/Texture/T_pattern_1.T_pattern_1'"));
	if (!IsValid(DissolvePatternTexture))
	{
		TRACE_PRINT_SCREEN_AND_LOG(TEXT("DissolvePatternTexture가 invalid합니다."));
	}
}

void UFacilityMaterialSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

UMaterialExpressionStaticBoolParameter* UFacilityMaterialSubsystem::CreateMaterialExpressionStaticBoolParameter(UMaterial* Material)
{
	return Cast<UMaterialExpressionStaticBoolParameter>(UMaterialEditingLibrary::CreateMaterialExpression(Material, UMaterialExpressionStaticBoolParameter::StaticClass()));;
}

UMaterialExpressionTextureObjectParameter* UFacilityMaterialSubsystem::CreateMaterialExpressionTextureObjectParameter(UMaterial* Material)
{
	return Cast<UMaterialExpressionTextureObjectParameter>(UMaterialEditingLibrary::CreateMaterialExpression(Material, UMaterialExpressionTextureObjectParameter::StaticClass()));;
}

UMaterialExpressionScalarParameter* UFacilityMaterialSubsystem::CreateMaterialExpressionScalarParameter(UMaterial* Material)
{
	return Cast<UMaterialExpressionScalarParameter>(UMaterialEditingLibrary::CreateMaterialExpression(Material, UMaterialExpressionScalarParameter::StaticClass()));;
}

UMaterialExpressionVectorParameter* UFacilityMaterialSubsystem::CreateMaterialExpressionVectorParameter(UMaterial* Material)
{
	return  Cast<UMaterialExpressionVectorParameter>(UMaterialEditingLibrary::CreateMaterialExpression(Material, UMaterialExpressionVectorParameter::StaticClass()));;
}

void UFacilityMaterialSubsystem::CreateOrUpdateDissolveMaterialFunctionNode(UMaterial* Material)
{
	if (!IsValid(Material))
	{
		TRACE_PRINT_SCREEN_AND_LOG(TEXT("Material이 invalid합니다."));
		return;
	}
	if (!IsValid(DissolveMaterialFunction))
	{
		TRACE_PRINT_SCREEN_AND_LOG(TEXT("DissolveMaterialFunction가 invalid합니다."));
		return;
	}

	Material->BlendMode = EBlendMode::BLEND_Masked;
	UMaterialEditorOnlyData* MaterialEditorOnlyData = Material->GetEditorOnlyData();
	if (!MaterialEditorOnlyData)
	{
		TRACE_PRINT_SCREEN_AND_LOG(Material->GetName() + TEXT(": MaterialEditorOnlyData캐스팅 실패"));
		return;
	}
	
	// Find or Create Dissolve Material Expression Function
	UMaterialExpressionMaterialFunctionCall* DissolveMaterialExpression;
	if (IsDissolveMaterialFunctionLinked(MaterialEditorOnlyData->EmissiveColor.Expression))
	{
		DissolveMaterialExpression = Cast<UMaterialExpressionMaterialFunctionCall>(MaterialEditorOnlyData->EmissiveColor.Expression);
	}
	else if (IsDissolveMaterialFunctionLinked(MaterialEditorOnlyData->OpacityMask.Expression))
	{
		DissolveMaterialExpression = Cast<UMaterialExpressionMaterialFunctionCall>(MaterialEditorOnlyData->OpacityMask.Expression);
	}
	else
	{
		DissolveMaterialExpression = Cast<UMaterialExpressionMaterialFunctionCall>(UMaterialEditingLibrary::CreateMaterialExpression(Material, UMaterialExpressionMaterialFunctionCall::StaticClass()));
		DissolveMaterialExpression->SetMaterialFunction(DissolveMaterialFunction);
		MaterialEditorOnlyData->ExpressionCollection.AddExpression(DissolveMaterialExpression);
		DissolveMaterialExpression->MaterialExpressionEditorX = -300;
		DissolveMaterialExpression->MaterialExpressionEditorY = +200;
	}

	if (!IsValid(DissolveMaterialExpression))
	{
		TRACE_PRINT_SCREEN(TEXT("DissolveMaterialExpression가 invalid합니다."));
		return;
	}

	MaterialEditorOnlyData->EmissiveColor.Connect(0, DissolveMaterialExpression);
	MaterialEditorOnlyData->OpacityMask.Connect(1, DissolveMaterialExpression);

	// Create Dissolve Material Expression Parameters
	TArray<FFunctionExpressionInput>& FunctionInputs = DissolveMaterialExpression->FunctionInputs;
	for (int i = 0; i < FunctionInputs.Num(); ++i)
	{
		FFunctionExpressionInput& FunctionExpressionInput = FunctionInputs[i]; 
		FName ExprParamName = GetDissolveMaterialFunctionParameterName(FunctionExpressionInput);
		if (FunctionExpressionInput.Input.IsConnected())
		{
			if (FunctionExpressionInput.Input.Expression->GetName() == ExprParamName)
			{
				continue;
			}
		}

		TEnumAsByte<EFunctionInputType> InputType = FunctionExpressionInput.ExpressionInput->InputType;
		UMaterialExpression* MaterialExpression = nullptr;
		switch (InputType)
		{
		case FunctionInput_StaticBool:
		{
			MaterialExpression = CreateMaterialExpressionStaticBoolParameter(Material);
			break;
		}
		case FunctionInput_Texture2D:
		{
			MaterialExpression = CreateMaterialExpressionTextureObjectParameter(Material);
			break;
		}
		case FunctionInput_Scalar:
		{
			MaterialExpression = CreateMaterialExpressionScalarParameter(Material);
			break;
		}
		case FunctionInput_Vector3:
		{
			MaterialExpression = CreateMaterialExpressionVectorParameter(Material);
			break;
		}
		default:
			TRACE_PRINT_SCREEN_AND_LOG(TEXT("정의되지 않은 InputType입니다.: ") + FString::FromInt(InputType));
			break;
		}
		
		if (MaterialExpression)
		{
			MaterialExpression->MaterialExpressionEditorX = DissolveMaterialExpression->MaterialExpressionEditorX - 300;
			MaterialExpression->MaterialExpressionEditorY = DissolveMaterialExpression->MaterialExpressionEditorY + 100 * (i - FunctionInputs.Num()/ 2);
			
			FunctionExpressionInput.Input.Connect(0, MaterialExpression);
		}

		if (UMaterialExpressionParameter* MaterialExpressionParam = Cast<UMaterialExpressionParameter>(MaterialExpression))
		{
			MaterialExpressionParam->SetParameterName(ExprParamName);
			MaterialExpressionParam->Group = TEXT("Dissolve");
		}
		if (UMaterialExpressionTextureSampleParameter* MaterialExpressionTextureSampleParam = Cast<UMaterialExpressionTextureSampleParameter>(MaterialExpression))
		{
			MaterialExpressionTextureSampleParam->SetParameterName(ExprParamName);
			MaterialExpressionTextureSampleParam->Group = TEXT("Dissolve");
		}
	}	

	// Update Dissolve Parameter Value


	Material->PostEditChange();
}

bool UFacilityMaterialSubsystem::IsDissolveMaterialFunctionLinkedToEmissiveOrOpacityMask(UMaterial* Material)
{
	if (UMaterialEditorOnlyData* EditorOnlyData = Material->GetEditorOnlyData())
	{
		return IsDissolveMaterialFunctionLinked(EditorOnlyData->EmissiveColor.Expression) ||
			IsDissolveMaterialFunctionLinked(EditorOnlyData->OpacityMask.Expression);
	}
	return false;
}

bool UFacilityMaterialSubsystem::IsDissolveMaterialFunctionLinked(UMaterialExpression* MaterialExpression)
{
	if (UMaterialExpressionMaterialFunctionCall* MaterialFunctionCall = Cast<UMaterialExpressionMaterialFunctionCall>(MaterialExpression))
	{
		if (UMaterialFunctionInterface* MaterialFunctionInterface = MaterialFunctionCall->MaterialFunction.Get())
		{
			if (IsValid(DissolveMaterialFunction))
			{
				return MaterialFunctionInterface->GetClass()->IsChildOf(DissolveMaterialFunction->GetClass());
			}
		}
	}
	return false;
}

FName UFacilityMaterialSubsystem::GetDissolveMaterialFunctionParameterName(const FFunctionExpressionInput& FunctionExpressionInput) const
{
	return FName(TEXT("DissolveMaterialFunction") + FunctionExpressionInput.Input.InputName.ToString());
}
