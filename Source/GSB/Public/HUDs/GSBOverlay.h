// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "GSBOverlay.generated.h"

class UCanvasPanel;
class UGSBWindowWidget;
class UGSBWindowBody;
class UGSBConfirmationDialog;
class UGSBDialogBody;
class UGSBNumberInputDialogBody;
class UGSBContextMenu;

UCLASS()
class GSB_API UGSBOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void InitializeOverlay();

	virtual UGSBWindowWidget* OpenWindow_Internal(UObject* InTargetObject);

	virtual void CloseWindow_Internal(UGSBWindowWidget* WindowWidget);	

	bool IsOpened_Internal(UGSBWindowWidget* WindowWidget) const;

	void CloseAllWindows_Internal();

	UGSBConfirmationDialog* OpenConfirmationDialog_Internal(UGSBDialogBody* DialogBody, UObject* InTargetObject);

	UGSBNumberInputDialogBody* OpenNumberInputDialog_Internal(UObject* InTargetObject, int Number = 0);

	UGSBContextMenu* OpenContextMenu_Internal(UObject* ContextTarget);

protected:

	UGSBConfirmationDialog* OpenConfirmationDialog_Internal(UObject* InTargetObject);

	UPROPERTY(EditDefaultsOnly, Category = "WindowWidget")
	TSubclassOf<UGSBWindowWidget> WindowWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "WindowWidget")
	TSubclassOf<UGSBConfirmationDialog> ConfirmationDialogWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "WindowWidget")
	TSubclassOf<UGSBNumberInputDialogBody> NumberInputDialogBodyWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "WindowWidget")
	TSubclassOf<UGSBContextMenu> ContextMenuWidgetClass;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* RootCanvas;

	TArray<UGSBWindowWidget*> WindowWidgets;
};