// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GSBWindowSubsystem.generated.h"

class UGSBWindow;
class AGSBHUD;
class UGSBContextMenu;
class UGSBNumberInputDialog;

UCLASS()
class GSB_API UGSBWindowSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UGSBWindow* OpenWindow(UClass* WindowClass, const FName& WindowName);

	UGSBNumberInputDialog* OpenNumberInputDialog(UClass* NumberInputDialogClass, const FName& DialogName, UObject* TargetObject);

	UGSBNumberInputDialog* OpenDefaultNumberInputDialog(const FName& DialogName, UObject* TargetObject);

	void CloseWindow(UGSBWindow* Window);

	bool IsWindowOpened(UGSBWindow* Window);

	void CloseAllWindows();

	UGSBContextMenu* OpenContextMenu(UClass* ContextMenuClass, const FName& ContextMenuName, UObject* ContextTarget);

	UGSBContextMenu* OpenDefaultContextMenu(const FName& ContextMenuName, UObject* ContextTarget);

private	:
	AGSBHUD* HUD;

	AGSBHUD* GetHUD();
};
