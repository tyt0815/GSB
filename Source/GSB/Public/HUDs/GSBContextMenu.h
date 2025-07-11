// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBWidget.h"
#include "GSBContextMenu.generated.h"

class UVerticalBox;
class UGSBContextMenuEntry;
class FGSBContextMenuEntryOnClickedSignature;

UCLASS()
class GSB_API UGSBContextMenu : public UGSBWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	virtual void NativeOnFocusChanging(const FWeakWidgetPath& PreviousFocusPath, const FWidgetPath& NewWidgetPath, const FFocusEvent& InFocusEvent) override;
	
public:
	FGSBContextMenuEntryOnClickedSignature* AddContextMenuEntry(const FName LabelText);

	void ClearContextMenuEntries();

	void CloseContextMenu();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "UGSBContextMenu")
	TSubclassOf<UGSBContextMenuEntry> ContextMenuEntryClass;

private:
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* EntryList;

public:
	FORCEINLINE UObject* GetContextTarget() const
	{
		return GetTargetObject();
	}
	FORCEINLINE void SetContextTarget(UObject* ContextTarget)
	{
		SetTargetObject(ContextTarget);
	}
};
