// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GSBOverlay.generated.h"

class UCanvasPanel;
class UGSBWindowWidget;
class UGSBWindowBody;
class UGSBConfirmationDialog;

UCLASS()
class GSB_API UGSBOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void InitializeOverlay();

	UGSBWindowWidget* OpenWindow(UGSBWindowBody* WindowBody);

	virtual void CloseWindow(UGSBWindowWidget* WindowWidget);

	UGSBConfirmationDialog* OpenConfirmationDialog();

	bool IsOpened(UGSBWindowWidget* WindowWidget) const;

	void CloseAllWindows();

protected:
	virtual UGSBWindowWidget* OpenWindow();

	UPROPERTY(EditDefaultsOnly, Category = "WindowWidget")
	TSubclassOf<UGSBWindowWidget> WindowWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "WindowWidget")
	TSubclassOf<UGSBConfirmationDialog> ConfirmationDialogWidgetClass;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* RootCanvas;

	TArray<UGSBWindowWidget*> WindowWidgets;
};
