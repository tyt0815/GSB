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
	static UGSBWindowSubsystem* Get(UObject* WorldContext);

	UGSBWindow* OpenWindow(UClass* WindowClass, const FName& WindowName);

	template<typename WindowT>
	void ToggleWindow(WindowT*& Window, UClass* WindowClass, const FName& WindowName);

	template<typename WindowT>
	void ToggleWindow(WindowT*& Window, const FName& WindowClassName, const FName& WindowName);

	UGSBNumberInputDialog* OpenNumberInputDialog(UClass* NumberInputDialogClass, const FName& DialogName, UObject* TargetObject);

	UGSBNumberInputDialog* OpenDefaultNumberInputDialog(const FName& DialogName, UObject* TargetObject);

	void CloseWindow(UGSBWindow* Window);

	bool IsWindowOpened(UGSBWindow* Window);

	void CloseAllWindows();

	UGSBContextMenu* OpenContextMenu(UClass* ContextMenuClass, const FName& ContextMenuName, UObject* ContextTarget);

	UGSBContextMenu* OpenDefaultContextMenu(const FName& ContextMenuName, UObject* ContextTarget);

private	:
	UGSBWindow* ToggleWindow_Internal(UGSBWindow* Window, UClass* WindowClass, const FName& WindowName);

	UGSBWindow* ToggleWindow_Internal(UGSBWindow* Window, const FName& WindowClassName, const FName& WindowName);

	UClass* GetWidgetClass(const FName& ClassName);

	AGSBHUD* HUD;

	AGSBHUD* GetHUD();
};

template<typename WindowT>
inline void UGSBWindowSubsystem::ToggleWindow(WindowT*& Window, UClass* WindowClass, const FName& WindowName)
{
	Window = Cast<WindowT>(ToggleWindow_Internal(Window, WindowClass, WindowName));
}

template<typename WindowT>
inline void UGSBWindowSubsystem::ToggleWindow(WindowT*& Window, const FName& WindowClassName, const FName& WindowName)
{
	Window = Cast<WindowT>(ToggleWindow_Internal(Window, WindowClassName, WindowName));
}
