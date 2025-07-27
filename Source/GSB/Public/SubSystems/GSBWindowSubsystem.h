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

	UGSBWindow* OpenWindow(UClass* WindowClass, const FName& DebugName);

	template<typename WindowT>
	void ToggleWindow(WindowT*& Window, UClass* WindowClass, const FName& DebugName);

	template<typename WindowT>
	void ToggleWindow(WindowT*& Window, const FName& WindowClassName, const FName& DebugName);

	UGSBNumberInputDialog* OpenNumberInputDialog(UClass* NumberInputDialogClass, UObject* TargetObject, const FName& DebugName);

	UGSBNumberInputDialog* OpenDefaultNumberInputDialog(UObject* TargetObject, const FName& DebugName);

	void CloseWindow(UGSBWindow* Window);

	bool IsWindowOpened(UGSBWindow* Window);

	void CloseAllWindowsOnCurrentOverlay();

	UGSBContextMenu* OpenContextMenu(UClass* ContextMenuClass, UObject* ContextTarget, const FName& DebugName);

	UGSBContextMenu* OpenDefaultContextMenu(UObject* ContextTarget, const FName& DebugName);

private	:
	UGSBWindow* ToggleWindow_Internal(UGSBWindow* Window, UClass* WindowClass, const FName& DebugName);

	UGSBWindow* ToggleWindow_Internal(UGSBWindow* Window, const FName& WindowClassName, const FName& DebugName);

	UClass* GetWidgetClass(const FName& ClassName);

	AGSBHUD* HUD;

	AGSBHUD* GetHUD();
};

template<typename WindowT>
inline void UGSBWindowSubsystem::ToggleWindow(WindowT*& Window, UClass* WindowClass, const FName& DebugName)
{
	Window = Cast<WindowT>(ToggleWindow_Internal(Window, WindowClass, DebugName));
}

template<typename WindowT>
inline void UGSBWindowSubsystem::ToggleWindow(WindowT*& Window, const FName& WindowClassName, const FName& DebugName)
{
	Window = Cast<WindowT>(ToggleWindow_Internal(Window, WindowClassName, DebugName));
}
