// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDs/GSBContextMenuEntry.h"
#include "GSBContextMenuWidget.generated.h"

class UVerticalBox;

UCLASS()
class GSB_API UGSBContextMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;

	virtual void NativeOnFocusChanging(const FWeakWidgetPath& PreviousFocusPath, const FWidgetPath& NewWidgetPath, const FFocusEvent& InFocusEvent) override;
	
public:
	FGSBContextMenuEntryOnClickedSignature* AddContextMenuEntry(const FName LabelText);

	void SetContextTarget(UWidget* ContextTarget);

	void ClearContextMenuEntries();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "UGSBContextMenuWidget")
	TSubclassOf<UGSBContextMenuEntry> ContextMenuEntryClass;

private:
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* EntryList;
};
