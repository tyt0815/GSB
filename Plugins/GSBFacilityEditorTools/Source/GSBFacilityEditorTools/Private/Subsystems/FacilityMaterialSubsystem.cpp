// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/FacilityMaterialSubsystem.h"
#include "Materials/MaterialExpressionFunctionInput.h"
#include "Materials/MaterialExpressionScalarParameter.h"
#include "Materials/MaterialExpressionStaticBoolParameter.h"
#include "Materials/MaterialExpressionVectorParameter.h"
#include "Materials/MaterialExpressionTextureObjectParameter.h"
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
		DissolveMaterialExpression = NewObject<UMaterialExpressionMaterialFunctionCall>(Material);
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


	TArray<FFunctionExpressionInput>& FunctionInputs = DissolveMaterialExpression->FunctionInputs;
	for (int i = 0; i < FunctionInputs.Num(); ++i)
	{
		FFunctionExpressionInput& FunctionExpressionInput = FunctionInputs[i];
		FName ExprParamName = FName(TEXT("DissolveMaterialFunction") + FunctionExpressionInput.Input.InputName.ToString());
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
			UMaterialExpressionStaticBoolParameter* StaticBoolParam = NewObject<UMaterialExpressionStaticBoolParameter>(Material);
			MaterialExpression = StaticBoolParam;
			break;
		}
		case FunctionInput_Texture2D:
		{
			UMaterialExpressionTextureObjectParameter* TextureObjParam = NewObject<UMaterialExpressionTextureObjectParameter>(Material);
			MaterialExpression = TextureObjParam;
			break;
		}
		case FunctionInput_Scalar:
		{
			UMaterialExpressionScalarParameter* ScalarParam = NewObject<UMaterialExpressionScalarParameter>(Material);
			MaterialExpression = ScalarParam;
			break;
		}
		case FunctionInput_Vector3:
		{
			UMaterialExpressionVectorParameter* VectorParam = NewObject <UMaterialExpressionVectorParameter>(Material);
			MaterialExpression = VectorParam;
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
		}
		if (UMaterialExpressionTextureSampleParameter* MaterialExpressionTextureSampleParam = Cast<UMaterialExpressionTextureSampleParameter>(MaterialExpression))
		{
			MaterialExpressionTextureSampleParam->SetParameterName(ExprParamName);
		}
		
	}	

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
