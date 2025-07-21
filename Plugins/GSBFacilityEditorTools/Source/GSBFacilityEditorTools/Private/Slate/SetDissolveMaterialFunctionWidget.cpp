// Fill out your copyright notice in the Description page of Project Settings.


#include "Slate/SetDissolveMaterialFunctionWidget.h"
#include "Subsystems/FacilityMaterialSubsystem.h"
#include "PropertyCustomizationHelpers.h"
#include "SDetailsSplitter.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "GSBDebugLibrary.h"

#define DEFAULT_DISSOLVE_PATTERN_TEXTURE_PATH TEXT("/GSBFacilityEditorTools/Texture/T_pattern_1.T_pattern_1")

void SSetDissolveMaterialFunctionWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	SelectedMaterials.Empty();

	DissolvePatternTexture = LoadObject<UTexture2D>(nullptr, DEFAULT_DISSOLVE_PATTERN_TEXTURE_PATH);

	for (const FAssetData& AssetData : InArgs._SelectedAssetDatas)
	{
		if (UClass* AssetClass = AssetData.GetClass())
		{
			if (AssetClass->IsChildOf<UMaterial>())
			{
				SelectedMaterials.Add(MakeShared<FAssetData>(AssetData));
			}
		}		
	}


	ChildSlot
		[
			SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					// Title
					SNew(STextBlock)
						.Text(FText::FromString(TEXT("Dissolve Material 추가")))
						.Justification(ETextJustify::Center)
						.ColorAndOpacity(FColor::White)
				]

				// Material AssetList
				+ SVerticalBox::Slot()
				.VAlign(EVerticalAlignment::VAlign_Fill)
				[
					SNew(SScrollBox)
						+SScrollBox::Slot()
						.Padding(32)
						[
							ConstructMaterialAssetListView()
						]
				]

				// Texture
				+SVerticalBox::Slot()
				.AutoHeight()
				.HAlign(EHorizontalAlignment::HAlign_Fill)
				.VAlign(EVerticalAlignment::VAlign_Top)
				[
					ConstructDissolveMaterialFunctionPropertyList()
				]
		];
}

TSharedRef<SListView<TSharedPtr<FAssetData>>> SSetDissolveMaterialFunctionWidget::ConstructMaterialAssetListView()
{
	ConstructedMaterialAssetListView = SNew(SListView<TSharedPtr<FAssetData>>)
		.ListItemsSource(&SelectedMaterials)
		.OnGenerateRow(this, &SSetDissolveMaterialFunctionWidget::OnGenerateRowForMaterialAssetListView)
		.OnMouseButtonDoubleClick(this, &SSetDissolveMaterialFunctionWidget::OnMaterialAssetListViewRowClicked);
	return ConstructedMaterialAssetListView.ToSharedRef();
}

TSharedRef<ITableRow> SSetDissolveMaterialFunctionWidget::OnGenerateRowForMaterialAssetListView(TSharedPtr<FAssetData> AssetDataToDisplay, const TSharedRef<STableViewBase>& OwnerTable)
{
	if (!AssetDataToDisplay.IsValid())
	{
		return SNew(STableRow<TSharedPtr<FAssetData>>, OwnerTable);
	}

	return SNew(STableRow<TSharedPtr<FAssetData>>, OwnerTable)
		.Padding(8)
		[
			SNew(STextBlock)
				.Text(FText::FromString(AssetDataToDisplay->AssetName.ToString()))
		];
}

void SSetDissolveMaterialFunctionWidget::OnMaterialAssetListViewRowClicked(TSharedPtr<FAssetData> AssetData)
{
	if (UMaterial* Material = Cast<UMaterial>(AssetData->GetAsset()))
	{
		if (GEditor)
		{
			if (UFacilityMaterialSubsystem* FMS = GEditor->GetEditorSubsystem<UFacilityMaterialSubsystem>())
			{
				FMS->CreateOrUpdateDissolveMaterialFunctionNode(Material);
			}
		}
	}
}

TSharedRef<SVerticalBox> SSetDissolveMaterialFunctionWidget::ConstructDissolveMaterialFunctionPropertyList()
{
	return SNew(SVerticalBox)
		// Pattern Texture
		+ SVerticalBox::Slot()
		[
			
			SNew(SSplitter)
				+ SSplitter::Slot()
				[
					// Property name
					SNew(STextBlock)
						.Text(FText::FromString("Pattern"))
						.Justification(ETextJustify::Center)
				]
				+ SSplitter::Slot()
				[
					SNew(SObjectPropertyEntryBox)
						.AllowedClass(UTexture::StaticClass())
						.ObjectPath(this, &SSetDissolveMaterialFunctionWidget::GetCurrentDissolvePatternTexturePath)
						.OnObjectChanged(this, &SSetDissolveMaterialFunctionWidget::OnDissolvePatternTextureChanged)
				]
		]
		+ SVerticalBox::Slot()
		[
			SNew(SSplitter)
				+SSplitter::Slot()
				[
					SNew(STextBlock)
						.Text(FText::FromString("Amount"))
						.Justification(ETextJustify::Center)
				]
				+ SSplitter::Slot()
				[
					SNew(SNumericEntryBox<float>)
						.LabelVAlign(VAlign_Center)
						.Value_Lambda([this]() { return TOptional<float>(Amount); })  // 현재 값 반환
						.OnValueChanged_Lambda([this](float NewValue) { Amount = NewValue; })  // 값 변경 처리
						.MinValue(0.f)
						.MaxValue(1.f)
						.MinSliderValue(0.f)
						.MaxSliderValue(1.f)
						.AllowSpin(true)
				]
		]
		;
}

FString SSetDissolveMaterialFunctionWidget::GetCurrentDissolvePatternTexturePath() const
{
	return IsValid(DissolvePatternTexture) ? DissolvePatternTexture->GetPathName() : FString();
}

void SSetDissolveMaterialFunctionWidget::OnDissolvePatternTextureChanged(const FAssetData& AssetData)
{
	if (UTexture2D* Texture = Cast<UTexture2D>(AssetData.GetAsset()))
	{
		DissolvePatternTexture = Texture;
	}
}