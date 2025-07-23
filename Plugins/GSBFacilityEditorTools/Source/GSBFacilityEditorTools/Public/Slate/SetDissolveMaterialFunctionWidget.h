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

	TSharedRef<SHorizontalBox> ConstructPropertyRowNameCell(const FText& PropertyName);

	TSharedRef<SSplitter> ConstructPropertyBooleanRow(const FText& PropertyName, bool& Value);

	TSharedRef<SSplitter> ConstructPropertyScalarRow(const FText& PropertyName, float& Value, float MinValue, float MaxValue);

	TSharedRef<SSplitter> ConstructPropertyColorRow(const FText& PropertyName, FLinearColor& Color);

	TSharedRef<SSplitter> ConstructPropertyTextureRow(const FText& PropertyName, UTexture*& Texture);

	FReply OnColorPropertyMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& PointerEvent, FLinearColor& Color);

	FString GetCurrentDissolvePatternTexturePath() const;

	void OnDissolvePatternTextureChanged(const FAssetData& AssetData);

	FReply OnConfirmButtonClicked();

	TArray<TSharedPtr<FAssetData>> SelectedMaterials;

	TSharedPtr<SListView<TSharedPtr<FAssetData>>> ConstructedMaterialAssetListView;

	bool bCreateMaterialInstance = true;

	bool bHueShift = false;

	bool bSwitchUVs = true;

	bool bUseOnlyTexture = false;

	float Amount = 0.6f;

	float Tiling = 4;

	float Width = 1;

	FLinearColor FringeColor = FLinearColor(0,1,1,1);

	UTexture* DissolvePatternTexture;
};
