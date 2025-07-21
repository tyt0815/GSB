// Fill out your copyright notice in the Description page of Project Settings.


#include "Slate/SetDissolveMaterialFunctionWidget.h"
#include "Subsystems/FacilityMaterialSubsystem.h"
#include "GSBDebugLibrary.h"

void SSetDissolveMaterialFunctionWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	SelectedMaterials.Empty();

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
