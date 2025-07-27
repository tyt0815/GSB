// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GSBOverlay.generated.h"

class UGSBOverlayPanel;
class UGSBWindow;
class UGSBNumberInputDialog;
class UGSBContextMenu;

UCLASS()
class GSB_API UGSBOverlay : public UUserWidget
{
	GENERATED_BODY()
public:
	void InitializeOverlay();

	UGSBWindow* OpenWindow(UClass* WindowClass, const FName& DebugName);

	UGSBWindow* ToggleWindow(UGSBWindow* Window, UClass* WindowClass, const FName& DebugName);

	UGSBNumberInputDialog* OpenNumberInputDialog(UClass* NumberInputDialogClass, UObject* TargetObject, const FName& DebugName);

	UGSBNumberInputDialog* OpenDefaultNumberInputDialog(UObject* TargetObject, const FName& DebugName);

	void CloseWindow(UGSBWindow* Window);

	bool IsWindowOpened(const UGSBWindow* Window) const;

	void CloseAllWindows();

	UGSBContextMenu* OpenContextMenu(UClass* ContextMenuClass, UObject* ContextTarget, const FName& DebugName);

	UGSBContextMenu* OpenDefaultContextMenu(UObject* ContextTarget, const FName& DebugName);
	
protected:
	UPROPERTY(meta = (BindWidget))
	UGSBOverlayPanel* OverlayPanel;
};