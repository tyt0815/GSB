// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDs/GSBContextMenuEntry.h"
#include "GSBContextMenu.generated.h"

class UVerticalBox;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGSBContextMenuClosed, UGSBContextMenu*, ContextMenu);

UCLASS()
class GSB_API UGSBContextMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	virtual void NativeOnFocusChanging(const FWeakWidgetPath& PreviousFocusPath, const FWidgetPath& NewWidgetPath, const FFocusEvent& InFocusEvent) override;
	
public:
	void OnOpened(UObject* InContextTarget);

	UGSBContextMenuEntry* AddContextMenuEntry(const FString& Label);

	void ClearContextMenuEntries();

	void CloseContextMenu();
	
	FOnGSBContextMenuClosed OnClosed;

private:
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* EntryList;

	UPROPERTY(EditDefaultsOnly, Category = "SubClasses")
	TSubclassOf<UGSBContextMenuEntry> ContextMenuEntryClass;

	UObject* ContextTarget;

public:
	FORCEINLINE UObject* GetContextTarget() const
	{
		return ContextTarget;
	}
};
