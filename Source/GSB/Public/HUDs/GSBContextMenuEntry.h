// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBWidget.h"
#include "GSBContextMenuEntry.generated.h"

class UButton;
class UTextBlock;
class UGSBContextMenuEntry;
class UGSBContextMenu;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGSBContextMenuEntryOnClickedSignature, UGSBContextMenuEntry*, MenuEntry);

UCLASS()
class GSB_API UGSBContextMenuEntry : public UGSBWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

public:
	FGSBContextMenuEntryOnClickedSignature ContextMenuEntryOnClicked;

	void SetLabelText(const FText& Text);

	void OnAddedtoContextMenu(UGSBContextMenu* InParentContextMenu, const FText& Text);
	
private:
	UPROPERTY(meta = (BindWidget))
	UButton* LabelButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* LabelText;

	UGSBContextMenu* ParentContextMenu;

	UFUNCTION()
	void HandleContextMenuEntryOnClicked();

public:
	FORCEINLINE UGSBContextMenu* GetParentContextMenu() const
	{
		return ParentContextMenu;
	}
};
