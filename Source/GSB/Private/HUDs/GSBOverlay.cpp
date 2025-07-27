// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBOverlay.h"
#include "HUDs/GSBOverlayPanel.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

void UGSBOverlay::InitializeOverlay()
{
	if (IsValid(OverlayPanel))
	{
		OverlayPanel->InitializeOverlayPanel();
	}
}

UGSBWindow* UGSBOverlay::OpenWindow(UClass* WindowClass, const FName& DebugName)
{
	if (IsValid(OverlayPanel))
	{
		return OverlayPanel->OpenWindow(WindowClass, DebugName);
	}
	return nullptr;
}

UGSBWindow* UGSBOverlay::ToggleWindow(UGSBWindow* Window, UClass* WindowClass, const FName& DebugName)
{
	if (IsValid(OverlayPanel))
	{
		return OverlayPanel->ToggleWindow(Window, WindowClass, DebugName);
	}
	return nullptr;
}

UGSBNumberInputDialog* UGSBOverlay::OpenNumberInputDialog(UClass* NumberInputDialogClass, UObject* TargetObject, const FName& DebugName)
{
	if (IsValid(OverlayPanel))
	{
		return OverlayPanel->OpenNumberInputDialog(NumberInputDialogClass, TargetObject, DebugName);
	}
	return nullptr;
}

UGSBNumberInputDialog* UGSBOverlay::OpenDefaultNumberInputDialog(UObject* TargetObject, const FName& DebugName)
{
	if (IsValid(OverlayPanel))
	{
		return OverlayPanel->OpenDefaultNumberInputDialog(TargetObject, DebugName);
	}
	return nullptr;
}

void UGSBOverlay::CloseWindow(UGSBWindow* Window)
{
	if (IsValid(OverlayPanel))
	{
		return OverlayPanel->CloseWindow(Window);
	}
}

bool UGSBOverlay::IsWindowOpened(const UGSBWindow* Window) const
{
	if (IsValid(OverlayPanel))
	{
		return OverlayPanel->IsWindowOpened(Window);
	}
	return false;
}

void UGSBOverlay::CloseAllWindows()
{
	if (IsValid(OverlayPanel))
	{
		return OverlayPanel->CloseAllWindows();
	}
}

UGSBContextMenu* UGSBOverlay::OpenContextMenu(UClass* ContextMenuClass, UObject* ContextTarget, const FName& DebugName)
{
	if (IsValid(OverlayPanel))
	{
		return OverlayPanel->OpenContextMenu(ContextMenuClass, ContextTarget, DebugName);
	}
	return nullptr;
}

UGSBContextMenu* UGSBOverlay::OpenDefaultContextMenu(UObject* ContextTarget, const FName& DebugName)
{
	if (IsValid(OverlayPanel))
	{
		return OverlayPanel->OpenDefaultContextMenu(ContextTarget, DebugName);
	}
	return nullptr;
}
