// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GSBContextMenuEntry.generated.h"

class UButton;
class UTextBlock;
class UGSBContextMenuEntry;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGSBContextMenuEntryOnClickedSignature, UWidget*, ContextTarget, UGSBContextMenuEntry*, MenuEntry);

UCLASS()
class GSB_API UGSBContextMenuEntry : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

public:
	FGSBContextMenuEntryOnClickedSignature ContextMenuEntryOnClicked;

	void SetLabelText(const FText& Text);
	
private:
	UPROPERTY(meta = (BindWidget))
	UButton* LabelButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* LabelText;

	UFUNCTION()
	void HandleContextMenuEntryOnClicked();

	UWidget* ContextTarget;

public:
	FORCEINLINE void SetContextTarget(UWidget* Widget)
	{
		ContextTarget = Widget;
	}
};
