// Fill out your copyright notice in the Description page of Project Settings.


#include "Slate/SetDissolveMaterialFunctionWidget.h"
#include "Subsystems/FacilityMaterialSubsystem.h"
#include "Widgets/Colors/SColorPicker.h"
#include "PropertyCustomizationHelpers.h"
#include "SDetailsSplitter.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "GSBDebugLibrary.h"

#define DEFAULT_DISSOLVE_PATTERN_TEXTURE_PATH TEXT("/GSBFacilityEditorTools/Texture/T_pattern_0.T_pattern_0")

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
				.Padding(32, 32, 32, 16)
				[
					// Title
					SNew(STextBlock)
						.Text(FText::FromString(TEXT("Dissolve Material 추가")))
						.Justification(ETextJustify::Center)
						.ColorAndOpacity(FColor::White)
				]

				// Material AssetList
				+ SVerticalBox::Slot()
				.HAlign(EHorizontalAlignment::HAlign_Fill)
				.VAlign(EVerticalAlignment::VAlign_Fill)
				.Padding(32, 16, 32, 16)
				[
					SNew(SScrollBox)
						+SScrollBox::Slot()
						[
							ConstructMaterialAssetListView()
						]
				]

				+SVerticalBox::Slot()
				.AutoHeight()
				.HAlign(EHorizontalAlignment::HAlign_Fill)
				.VAlign(EVerticalAlignment::VAlign_Bottom)
				.Padding(32, 0, 32, 8)
				[
					ConstructDissolveMaterialFunctionPropertyList()
				]

				+ SVerticalBox::Slot()
				.AutoHeight()
				.HAlign(EHorizontalAlignment::HAlign_Fill)
				.VAlign(EVerticalAlignment::VAlign_Bottom)
				.Padding(32, 8, 32, 8)
				[
					ConstructPropertyBooleanRow(FText::FromString(TEXT("Create Material instance")), bCreateMaterialInstance)
				]

				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(32, 8, 32, 32)
				.HAlign(EHorizontalAlignment::HAlign_Center)
				.VAlign(EVerticalAlignment::VAlign_Bottom)
				[
					SNew(SButton)
					.Text(FText::FromString(TEXT("확인")))
					.OnClicked(this, &SSetDissolveMaterialFunctionWidget::OnConfirmButtonClicked)
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
	
}

TSharedRef<SVerticalBox> SSetDissolveMaterialFunctionWidget::ConstructDissolveMaterialFunctionPropertyList()
{
	return SNew(SVerticalBox)
		+SVerticalBox::Slot()
		[
			ConstructPropertyBooleanRow(FText::FromString(TEXT("HueShift")), bHueShift)
		]
		+SVerticalBox::Slot()
		[
			ConstructPropertyBooleanRow(FText::FromString(TEXT("SwitchUVs")), bSwitchUVs)
		]
		+SVerticalBox::Slot()
		[
			ConstructPropertyBooleanRow(FText::FromString(TEXT("UseOnlyTexture")), bUseOnlyTexture)
		]

		+ SVerticalBox::Slot()
		[
			ConstructPropertyScalarRow(FText::FromString(TEXT("Amount")), Amount, 0, 1)
		]
		+ SVerticalBox::Slot()
		[
			ConstructPropertyScalarRow(FText::FromString(TEXT("Tiling")), Tiling, 0, 256)
		]
		+ SVerticalBox::Slot()
		[
			ConstructPropertyScalarRow(FText::FromString(TEXT("Width")), Width, 0, 256)
		]

		+ SVerticalBox::Slot()
		[
			ConstructPropertyColorRow(FText::FromString(TEXT("FringeColor")), FringeColor)
		]

		+ SVerticalBox::Slot()
		[
			ConstructPropertyTextureRow(FText::FromString(TEXT("Pattern")), DissolvePatternTexture)
		]
		;
}

TSharedRef<SHorizontalBox> SSetDissolveMaterialFunctionWidget::ConstructPropertyRowNameCell(const FText& PropertyName)
{
	return SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.VAlign(EVerticalAlignment::VAlign_Center)
		.HAlign(EHorizontalAlignment::HAlign_Left)
		.Padding(32, 0, 0, 0)
		
		[
			SNew(STextBlock)
				.Text(PropertyName)
				.Justification(ETextJustify::Center)
		];
}

TSharedRef<SSplitter> SSetDissolveMaterialFunctionWidget::ConstructPropertyBooleanRow(const FText& PropertyName, bool& Value)
{
	TSharedRef<SCheckBox> CheckBox = SNew(SCheckBox)
		.Type(ESlateCheckBoxType::CheckBox)
		.OnCheckStateChanged_Lambda([&Value](ECheckBoxState State)
			{
				switch (State)
				{
				case ECheckBoxState::Unchecked:
					Value = false;
					break;
				case ECheckBoxState::Checked:
					Value = true;
					break;
				case ECheckBoxState::Undetermined:
					break;
				default:
					break;
				}
			}
		);

	if(CheckBox->IsChecked() != Value)
	{ 
		CheckBox->ToggleCheckedState();
	}

	return SNew(SSplitter)
		+ SSplitter::Slot()
		[
			ConstructPropertyRowNameCell(PropertyName)
		]
		+ SSplitter::Slot()
		[
			SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.VAlign(EVerticalAlignment::VAlign_Center)
				.HAlign(EHorizontalAlignment::HAlign_Fill)
				.MaxWidth(100)
				[
					CheckBox
				]

		];
}

TSharedRef<SSplitter> SSetDissolveMaterialFunctionWidget::ConstructPropertyScalarRow(const FText& PropertyName, float& Value, float MinValue, float MaxValue)
{
	return SNew(SSplitter)
		+ SSplitter::Slot()
		[
			ConstructPropertyRowNameCell(PropertyName)
		]
		+ SSplitter::Slot()
		[
			SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.VAlign(EVerticalAlignment::VAlign_Center)
				.HAlign(EHorizontalAlignment::HAlign_Fill)
				.MaxWidth(100)
				[
					SNew(SNumericEntryBox<float>)
						.LabelVAlign(VAlign_Center)
						.Value_Lambda([&Value]() { return Value; })  // 현재 값 반환
						.OnValueChanged_Lambda([&Value](float NewValue) { Value = NewValue; })  // 값 변경 처리
						.MinValue(MinValue)
						.MaxValue(MaxValue)
						.MinSliderValue(MinValue)
						.MaxSliderValue(MaxValue)
						.AllowSpin(true)
				]

		];
}

TSharedRef<SSplitter> SSetDissolveMaterialFunctionWidget::ConstructPropertyColorRow(const FText& PropertyName, FLinearColor& Color)
{
	return SNew(SSplitter)
		+ SSplitter::Slot()
		[
			ConstructPropertyRowNameCell(PropertyName)
		]
		+ SSplitter::Slot()
		[
			SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.VAlign(EVerticalAlignment::VAlign_Center)
				.HAlign(EHorizontalAlignment::HAlign_Fill)
				.MaxWidth(100)
				[
					SNew(SColorBlock)
						.Color_Lambda([&Color]() { return Color; })
						.OnMouseButtonDown_Lambda([this, &Color](const FGeometry& Geometry, const FPointerEvent& PointerEvent)
							{
								return this->OnColorPropertyMouseButtonDown(Geometry, PointerEvent, Color);
							}
						)
				]
		];
}

TSharedRef<SSplitter> SSetDissolveMaterialFunctionWidget::ConstructPropertyTextureRow(const FText& PropertyName, UTexture*& Texture)
{
	return SNew(SSplitter)
		+ SSplitter::Slot()
		[
			ConstructPropertyRowNameCell(PropertyName)
		]
		+ SSplitter::Slot()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.VAlign(EVerticalAlignment::VAlign_Center)
				.HAlign(EHorizontalAlignment::HAlign_Fill)
				.MaxWidth(200)
				[
					SNew(SObjectPropertyEntryBox)
					.AllowedClass(UTexture::StaticClass())
					.ObjectPath_Lambda([&Texture]()
						{
							return IsValid(Texture) ? Texture->GetPathName() : FString();
						}
					)
					.OnObjectChanged_Lambda([&Texture](const FAssetData& AssetData)
						{
							Texture = Cast<UTexture>(AssetData.GetAsset());
						}
					)
				]
			];
}

FReply SSetDissolveMaterialFunctionWidget::OnColorPropertyMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& PointerEvent, FLinearColor& Color)
{
	FColorPickerArgs PickerArgs;
	PickerArgs.bIsModal = true;
	PickerArgs.InitialColor = Color;
	PickerArgs.OnColorCommitted.BindLambda([&Color](FLinearColor NewColor) { Color = NewColor; });
	OpenColorPicker(PickerArgs);
	return FReply::Handled();
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

FReply SSetDissolveMaterialFunctionWidget::OnConfirmButtonClicked()
{
	if (GEditor)
	{
		if (UFacilityMaterialSubsystem* FMS = GEditor->GetEditorSubsystem<UFacilityMaterialSubsystem>())
		{
			UFacilityMaterialSubsystem::FDissolveMaterialFunctionParameters Params = {};
			Params.bCreateMaterialInstance = bCreateMaterialInstance;
			Params.bHueShift = bHueShift;
			Params.bSwitchUVs = bSwitchUVs;
			Params.bUseOnlyTexture = bUseOnlyTexture;
			Params.Amount = Amount;
			Params.Tiling = Tiling;
			Params.Width = Width;
			Params.FringeColor = FringeColor;
			Params.Pattern = DissolvePatternTexture;
			FMS->CreateOrUpdateDissolveMaterialAssets(SelectedMaterials, Params);
		}
	}
	
	return FReply::Handled();
}
