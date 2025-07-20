// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetActions/GSBFacilityMaterialAssetAction.h"
#include "GSBDebugLibrary.h"


UGSBFacilityMaterialAssetAction::UGSBFacilityMaterialAssetAction()
{
	RegisterSetDissolveMaterialFunctionTab();
}

void UGSBFacilityMaterialAssetAction::SetDissolveMaterialFunction()
{
	FGlobalTabmanager::Get()->TryInvokeTab(FName(TEXT("SetDissolveMaterialFunction")));
}

void UGSBFacilityMaterialAssetAction::RegisterSetDissolveMaterialFunctionTab()
{
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
		FName("SetDissolveMaterialFunction"), 
		FOnSpawnTab::CreateUObject(this, &UGSBFacilityMaterialAssetAction::HandleOnSpawnSetDissolveMaterialFunctionTab)
	)
		.SetDisplayName(FText::FromString(TEXT("Set Dissolve Material Function")));
}

TSharedRef<SDockTab> UGSBFacilityMaterialAssetAction::HandleOnSpawnSetDissolveMaterialFunctionTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		;
}
