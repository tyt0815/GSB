// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GSBContextMenuEntry.generated.h"

class UGSBContextMenuEntry;
class UGSBContextMenu;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnContextMenuEntryClickedSignature, UGSBContextMenuEntry*, Entry);

UCLASS()
class GSB_API UGSBContextMenuEntry : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

public:
	void SetLabelText(const FText& Text);

	void OnAddedtoContextMenu(UGSBContextMenu* InParentContextMenu, const FText& Text);

	UObject* GetContextTarget() const;

	void CloseContextMenu();

	FOnContextMenuEntryClickedSignature OnClicked;
	
private:
	UFUNCTION()
	void HandleOnLabelButtonClicked();

	UPROPERTY(meta = (BindWidget))
	class UButton* LabelButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LabelText;

	UGSBContextMenu* ParentContextMenu;

public:
	FORCEINLINE UGSBContextMenu* GetParentContextMenu() const
	{
		return ParentContextMenu;
	}
};
