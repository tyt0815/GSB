// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetActions/AddPrefixAssetAction.h"
#include "EditorUtilityLibrary.h"
#include "DebugHeader.h"

void UAddPrefixAssetAction::AddPrefix()
{
	TArray<UObject*> SelectedObjects = UEditorUtilityLibrary::GetSelectedAssets();
	uint32 Counter = 0;
	for (UObject* SelectedObject : SelectedObjects)
	{
		if (!IsValid(SelectedObject))
		{
			continue;
		}

		FString PreFix = GetPrefixByObject(SelectedObject);
		if (PreFix.IsEmpty())
		{
			TyTDebug::PrintOnScreen(TEXT("Failed to find prefix for class") + SelectedObject->GetClass()->GetName(), FColor::Red);
			continue;
		}

		FString OldName = SelectedObject->GetName();
		if (OldName.StartsWith(PreFix))
		{
			TyTDebug::PrintOnScreen(OldName + TEXT(" alreay has prefix added"), FColor::Red);
			continue;
		}

		FString NewName = OldName;
		if (SelectedObject->IsA< UMaterialInstanceConstant>())
		{
			FString MaterialPrefix = GetPrefix(UMaterial::StaticClass());
			NewName.RemoveFromStart(MaterialPrefix);
			NewName.RemoveFromEnd(TEXT("_Inst"));
		}
		const FString NewNameWithPrefix = PreFix + NewName;

		UEditorUtilityLibrary::RenameAsset(SelectedObject, NewNameWithPrefix);
		++Counter;
	}

	if (Counter > 0)
	{
		TyTDebug::ShowNotifyInfo(TEXT("Successfully renamed ") + FString::FromInt(Counter) + " assets");
	}
}

FString UAddPrefixAssetAction::GetPrefixByObject(UObject* Object)
{
	if (IsValid(Object))
	{
		return GetPrefix(Object->GetClass());
	}
	return FString();
}

FString UAddPrefixAssetAction::GetPrefix(UClass* Class)
{
	if (PrefixMap.Contains(Class))
	{
		return *PrefixMap[Class];
	}
	return FString();
}
