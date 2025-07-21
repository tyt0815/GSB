// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Misc/Optional.h"
#include "Widgets/SCompoundWidget.h"

class GSBFACILITYEDITORTOOLS_API SSetDissolveMaterialFunctionWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SSetDissolveMaterialFunctionWidget) {}
		SLATE_ARGUMENT(TArray<FAssetData>, SelectedAssetDatas)
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs);

private	:
	TSharedRef<SListView<TSharedPtr<FAssetData>>> ConstructMaterialAssetListView();

	TSharedRef<ITableRow> OnGenerateRowForMaterialAssetListView(TSharedPtr<FAssetData> AssetDataToDisplay, const TSharedRef<STableViewBase>& OwnerTable);

	void OnMaterialAssetListViewRowClicked(TSharedPtr<FAssetData> AssetData);

	TSharedRef<SVerticalBox> ConstructDissolveMaterialFunctionPropertyList();

	FString GetCurrentDissolvePatternTexturePath() const;

	void OnDissolvePatternTextureChanged(const FAssetData& AssetData);

	TArray<TSharedPtr<FAssetData>> SelectedMaterials;

	TSharedPtr<SListView<TSharedPtr<FAssetData>>> ConstructedMaterialAssetListView;

	UTexture2D* DissolvePatternTexture;

	float Amount = 1;

	float Tilting = 1;
};
