// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/FacilityMaterialSubsystem.h"
#include "AssetToolsModule.h"
#include "EditorAssetLibrary.h"
#include "EditorSupportDelegates.h"
#include "Factories/MaterialFactoryNew.h"
#include "Factories/MaterialInstanceConstantFactoryNew.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialExpressionFunctionInput.h"
#include "Materials/MaterialExpressionScalarParameter.h"
#include "Materials/MaterialExpressionStaticBoolParameter.h"
#include "Materials/MaterialExpressionVectorParameter.h"
#include "Materials/MaterialExpressionTextureObjectParameter.h"
#include "Materials/MaterialExpressionLinearInterpolate.h"
#include "Materials/MaterialExpressionMin.h"
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

UMaterialInstanceConstant* UFacilityMaterialSubsystem::CreateMaterialInstanceConstant(const FString& NameOfTheMaterialInst, const FString PackagePath)
{
	UMaterialInstanceConstantFactoryNew* MaterialInstFactory = NewObject< UMaterialInstanceConstantFactoryNew>();
	FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools"));
	UObject* CreatedObject = AssetToolsModule.Get().CreateAsset(NameOfTheMaterialInst, PackagePath, UMaterialInstanceConstant::StaticClass(), MaterialInstFactory);
	UMaterialInstanceConstant* CreatedMaterialInst = nullptr;
	CreatedMaterialInst = Cast<UMaterialInstanceConstant>(CreatedObject);
	return CreatedMaterialInst;

	return nullptr;
}

UMaterialInstanceConstant* UFacilityMaterialSubsystem::FindOrCreateMaterialInstanceConstant(const FString& NameOfTheMaterialInst, const FString PackagePath)
{
	if (UEditorAssetLibrary::DoesAssetExist(PackagePath / NameOfTheMaterialInst))
	{
		return Cast<UMaterialInstanceConstant>(UEditorAssetLibrary::FindAssetData(PackagePath / NameOfTheMaterialInst).GetAsset());
	}
	return CreateMaterialInstanceConstant(NameOfTheMaterialInst, PackagePath);
}

UMaterialExpression* UFacilityMaterialSubsystem::FindMaterialExpressionByDesc(UMaterial* Material, const FString& Desc)
{
	Material->GetExpressions();
	if (UMaterialEditorOnlyData* EditorOnly = Material->GetEditorOnlyData())
	{
		TArray<TObjectPtr<UMaterialExpression>>& ExprCollection = EditorOnly->ExpressionCollection.Expressions;
		for (const TObjectPtr<UMaterialExpression>& MatExpr : ExprCollection)
		{
			if (IsValid(MatExpr) && MatExpr->Desc == Desc)
			{
				return MatExpr.Get();
			}
		}
	}
	return nullptr;
}

UMaterialExpressionParameter* UFacilityMaterialSubsystem::FindMaterialExpressionParameterByName(UMaterial* Material, const FName& ParameterName)
{
	for (const TObjectPtr<UMaterialExpression>& MatExpr : Material->GetExpressions())
	{
		if (UMaterialExpressionParameter* MatExprParam = Cast<UMaterialExpressionParameter>(MatExpr))
		{
			if (MatExprParam->GetParameterName() == ParameterName)
			{
				return MatExprParam;
			}
		}
	}
	return nullptr;
}

UMaterialExpressionTextureSampleParameter* UFacilityMaterialSubsystem::FindMaterialExpressionTextureSampleParameterByName(UMaterial* Material, const FString& ParameterName)
{
	for (const TObjectPtr<UMaterialExpression>& MatExpr : Material->GetExpressions())
	{
		if (UMaterialExpressionTextureSampleParameter* MatExprTextureSampleParam = Cast<UMaterialExpressionTextureSampleParameter>(MatExpr))
		{
			if (MatExprTextureSampleParam->GetParameterName() == ParameterName)
			{
				return MatExprTextureSampleParam;
			}
		}
	}
	return nullptr;
}

void UFacilityMaterialSubsystem::CreateOrUpdateDissolveMaterialFunctionNode(UMaterial* Material, const FDissolveMaterialFunctionParameters& Params)
{
	if (!IsValid(Material))
	{
		TRACE_PRINT_SCREEN_AND_LOG(TEXT("Material이 invalid합니다."));
		return;
	}
	if (!IsValid(DissolveMaterialFunction))
	{
		TRACE_PRINT_SCREEN_AND_LOG(TEXT("DissolveMaterialFunction이 invalid합니다."));
		return;
	}

	Material->BlendMode = EBlendMode::BLEND_Masked;
	UMaterialEditorOnlyData* MaterialEditorOnlyData = Material->GetEditorOnlyData();
	if (!MaterialEditorOnlyData)
	{
		TRACE_PRINT_SCREEN_AND_LOG(Material->GetName() + TEXT(": MaterialEditorOnlyData가 nullptr 입니다."));
		return;
	}
	
	// Find or Create Dissolve Material Expression Function
	UMaterialExpressionMaterialFunctionCall* DissolveMaterialExpression;
	//// Find
	if (UMaterialExpression* MatExpr = FindMaterialExpressionByDesc(Material, TEXT("Final Dissolve Effect")))
	{
		DissolveMaterialExpression = Cast<UMaterialExpressionMaterialFunctionCall>(MatExpr);
	}
	//// Create
	else
	{
		DissolveMaterialExpression = Cast<UMaterialExpressionMaterialFunctionCall>(UMaterialEditingLibrary::CreateMaterialExpression(Material, UMaterialExpressionMaterialFunctionCall::StaticClass()));
		DissolveMaterialExpression->SetMaterialFunction(DissolveMaterialFunction);
		DissolveMaterialExpression->Desc = TEXT("Final Dissolve Effect");
		DissolveMaterialExpression->MaterialExpressionEditorX -= 600;
		DissolveMaterialExpression->MaterialExpressionEditorY += 300;

		// ConnectPin To EmissiveColor and OpacityMask
		if (MaterialEditorOnlyData->EmissiveColor.IsConnected())
		{
			FColorMaterialInput& EmissiveColorInput = MaterialEditorOnlyData->EmissiveColor;
			UMaterialExpressionLinearInterpolate* MatExprLerp = CreateMaterialExpression<UMaterialExpressionLinearInterpolate>(Material);
			MatExprLerp->B.Connect(EmissiveColorInput.OutputIndex, EmissiveColorInput.Expression);
			MatExprLerp->A.Connect(0, DissolveMaterialExpression);		// EmissiveColor
			MatExprLerp->Alpha.Connect(1, DissolveMaterialExpression);	// OpacityMask
			MatExprLerp->MaterialExpressionEditorX = DissolveMaterialExpression->MaterialExpressionEditorX + 300;
			MatExprLerp->MaterialExpressionEditorY = DissolveMaterialExpression->MaterialExpressionEditorY - 300;
			EmissiveColorInput.Connect(0, MatExprLerp);
		}
		else
		{
			MaterialEditorOnlyData->EmissiveColor.Connect(0, DissolveMaterialExpression);
		}
		if (MaterialEditorOnlyData->OpacityMask.IsConnected())
		{
			FScalarMaterialInput& OpacityMaskInput = MaterialEditorOnlyData->OpacityMask;
			UMaterialExpressionMin* MatExprMin = CreateMaterialExpression<UMaterialExpressionMin>(Material);
			MatExprMin->A.Connect(OpacityMaskInput.OutputIndex, OpacityMaskInput.Expression);
			MatExprMin->B.Connect(1, DissolveMaterialExpression);
			MatExprMin->MaterialExpressionEditorX = DissolveMaterialExpression->MaterialExpressionEditorX + 300;
			MatExprMin->MaterialExpressionEditorY = DissolveMaterialExpression->MaterialExpressionEditorY - 150;
			OpacityMaskInput.Connect(0, MatExprMin);
		}
		else
		{
			MaterialEditorOnlyData->OpacityMask.Connect(1, DissolveMaterialExpression);
		}
	}

	if (!IsValid(DissolveMaterialExpression))
	{
		TRACE_PRINT_SCREEN(TEXT("DissolveMaterialExpression가 invalid합니다."));
		return;
	}

	

	TArray<FFunctionExpressionInput>& FunctionInputs = DissolveMaterialExpression->FunctionInputs;
	for (int i = 0; i < FunctionInputs.Num(); ++i)
	{
		FFunctionExpressionInput& FunctionExpressionInput = FunctionInputs[i]; 
		FName ExprParamName = GetDissolveMaterialFunctionParameterName(FunctionExpressionInput);

		// 파라미터가 연결되었는지 확인하는 방식 수정이 필요할 수도 있음. IsConnected함수가 아무 노드도 연결되어 있지
		// 않아도 true를 반환하여 이름을 비교하는 방식을 채택. 아마 머티리얼 함수의 Default값이 설정되어 있어서 IsConnected가
		// true를 반환한 것으로 추정됨
		if (FunctionExpressionInput.Input.IsConnected())
		{
			if (UMaterialExpressionParameter* ExprParam = Cast<UMaterialExpressionParameter>(FunctionExpressionInput.Input.Expression))
			{
				if (ExprParam->GetParameterName() == ExprParamName)
				{
					continue;
				}
			}

			if (UMaterialExpressionTextureSampleParameter* ExprParam = Cast<UMaterialExpressionTextureSampleParameter>(FunctionExpressionInput.Input.Expression))
			{
				if (ExprParam->GetParameterName() == ExprParamName)
				{
					continue;
				}
			}
		}

		// 타입에 따른 머티리얼 표현식 파라미터 생성
		TEnumAsByte<EFunctionInputType> InputType = FunctionExpressionInput.ExpressionInput->InputType;
		UMaterialExpression* MaterialExpression = nullptr;
		switch (InputType)
		{
		case FunctionInput_StaticBool:
		{
			MaterialExpression = CreateMaterialExpression<UMaterialExpressionStaticBoolParameter>(Material);
			break;
		}
		case FunctionInput_Scalar:
		{
			MaterialExpression = CreateMaterialExpression<UMaterialExpressionScalarParameter>(Material);
			break;
		}
		case FunctionInput_Vector3:
		{
			MaterialExpression = CreateMaterialExpression<UMaterialExpressionVectorParameter>(Material);
			break;
		}
		case FunctionInput_Texture2D:
		{
			MaterialExpression = CreateMaterialExpression<UMaterialExpressionTextureObjectParameter>(Material);
			break;
		}
		default:
			TRACE_PRINT_SCREEN_AND_LOG(TEXT("정의되지 않은 InputType입니다.: ") + FString::FromInt(InputType));
			break;
		}
		
		// 텍스처의 경우, UMaterialExpressionParameter를 상속하지 않고 독자적으로 UMaterialExpressionTextureSampleParameter
		// 를 상속하기 때문에 아래와 같이 분리하여 처리함.
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
	// Create Dissolve Material Expression Parameters
	if (UMaterialExpressionStaticBoolParameter* MatExpr = Cast<UMaterialExpressionStaticBoolParameter>(FindMaterialExpressionParameterByName(Material, TEXT("DissolveEffect_HueShift"))))
	{
		MatExpr->SortPriority = 0;
		MatExpr->DefaultValue = Params.bHueShift;
	}
	if(UMaterialExpressionStaticBoolParameter* MatExpr = Cast<UMaterialExpressionStaticBoolParameter>(FindMaterialExpressionParameterByName(Material, TEXT("DissolveEffect_SwitchUVs"))))
	{
		MatExpr->SortPriority = 0;
		MatExpr->DefaultValue = Params.bSwitchUVs;
	}
	if(UMaterialExpressionStaticBoolParameter* MatExpr = Cast<UMaterialExpressionStaticBoolParameter>(FindMaterialExpressionParameterByName(Material, TEXT("DissolveEffect_UseOnlyTexture"))))
	{
		MatExpr->SortPriority = 0;
		MatExpr->DefaultValue = Params.bUseOnlyTexture;
	}
	if (UMaterialExpressionScalarParameter* MatExpr = Cast<UMaterialExpressionScalarParameter>(FindMaterialExpressionParameterByName(Material, TEXT("DissolveEffect_Amount"))))
	{
		MatExpr->SortPriority = 1;
		MatExpr->DefaultValue = Params.Amount;
	}
	if (UMaterialExpressionScalarParameter* MatExpr = Cast<UMaterialExpressionScalarParameter>(FindMaterialExpressionParameterByName(Material, TEXT("DissolveEffect_Tiling"))))
	{
		MatExpr->SortPriority = 1;
		MatExpr->DefaultValue = Params.Tiling;
	}
	if (UMaterialExpressionScalarParameter* MatExpr = Cast<UMaterialExpressionScalarParameter>(FindMaterialExpressionParameterByName(Material, TEXT("DissolveEffect_Width"))))
	{
		MatExpr->SortPriority = 1;
		MatExpr->DefaultValue = Params.Width;
	}
	if (UMaterialExpressionVectorParameter* MatExpr = Cast<UMaterialExpressionVectorParameter>(FindMaterialExpressionParameterByName(Material, TEXT("DissolveEffect_FringeColor"))))
	{
		MatExpr->SortPriority = 2;
		MatExpr->DefaultValue = Params.FringeColor;
	}
	if (UMaterialExpressionTextureObjectParameter* MatExpr = Cast<UMaterialExpressionTextureObjectParameter>(FindMaterialExpressionTextureSampleParameterByName(Material, TEXT("DissolveEffect_Pattern"))))
	{
		MatExpr->SortPriority = 3;
		MatExpr->Texture = Params.Pattern;
	}

	Material->PreEditChange(nullptr);
	Material->PostEditChange();
	if (GEditor)
	{
		GEditor->OnSceneMaterialsModified();
	}
	UMaterialEditingLibrary::RecompileMaterial(Material);
}

FAssetData UFacilityMaterialSubsystem::FindOrCreateAssetWithSuffix(const FAssetData& AssetData, FString Suffix)
{
	if (!Suffix.StartsWith(TEXT("_")))
	{
		Suffix = TEXT("_") + Suffix;
	}

	if (AssetData.AssetName.ToString().EndsWith(Suffix))
	{
		return AssetData;
	}
	else
	{
		const FString AssetNameWithSuffix = AssetData.AssetName.ToString() + Suffix;
		const FString PackagePath = AssetData.PackagePath.ToString();
		const FString TargetAssetPath = PackagePath / AssetNameWithSuffix;
		if (!UEditorAssetLibrary::DoesAssetExist(TargetAssetPath))
		{
			const FString SourceAssetPath = AssetData.GetSoftObjectPath().ToString();

			if (UEditorAssetLibrary::DuplicateAsset(SourceAssetPath, TargetAssetPath))
			{
				UEditorAssetLibrary::SaveAsset(TargetAssetPath, false);
			}
		}
		return UEditorAssetLibrary::FindAssetData(TargetAssetPath);
	}
}

FAssetData UFacilityMaterialSubsystem::CreateOrUpdateDissolveMaterialAsset(const FAssetData& MaterialAssetData, const FDissolveMaterialFunctionParameters& Params)
{
	if (!MaterialAssetData.GetAsset()->GetClass()->IsChildOf<UMaterial>())
	{
		return FAssetData();
	}

	FAssetData DissolveMaterialAsset = FindOrCreateAssetWithSuffix(MaterialAssetData, TEXT("Dissolve"));

	CreateOrUpdateDissolveMaterialFunctionNode(Cast<UMaterial>(DissolveMaterialAsset.GetAsset()), Params);

	UEditorAssetLibrary::SaveAsset(DissolveMaterialAsset.GetSoftObjectPath().ToString());

	if (Params.bCreateMaterialInstance && DissolveMaterialAsset.IsValid())
	{
		FString MatInstName = DissolveMaterialAsset.AssetName.ToString();
		MatInstName.RemoveFromStart(TEXT("M_"));
		MatInstName.InsertAt(0, TEXT("MI_"));
		if (UMaterialInstanceConstant* MaterialInstanceConst = FindOrCreateMaterialInstanceConstant(MatInstName, DissolveMaterialAsset.PackagePath.ToString()))
		{
			if (MaterialInstanceConst->Parent != DissolveMaterialAsset.GetAsset())
			{
				MaterialInstanceConst->SetParentEditorOnly(Cast<UMaterialInterface>(DissolveMaterialAsset.GetAsset()));
				MaterialInstanceConst->PostEditChange();
			}
		}
	}

	return DissolveMaterialAsset;
}

void UFacilityMaterialSubsystem::CreateOrUpdateDissolveMaterialAssets(TConstArrayView<TSharedPtr<FAssetData>> MaterialAssetDatas, const FDissolveMaterialFunctionParameters& Params)
{
	for (const TSharedPtr<FAssetData>& MaterialAssetData : MaterialAssetDatas)
	{
		CreateOrUpdateDissolveMaterialAsset(*MaterialAssetData.Get(), Params);
	}
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
	return FName(TEXT("DissolveEffect_") + FunctionExpressionInput.Input.InputName.ToString());
}
