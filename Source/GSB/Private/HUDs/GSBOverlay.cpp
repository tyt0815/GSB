// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBOverlay.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"


void UGSBOverlay::InitializeOverlay()
{
}

UGSBWindow* UGSBOverlay::OpenWindow(TSubclassOf<UGSBWindow> WindowClass, const FName& WindowName)
{
	if (UGSBWindow* Window = CreateWidget_GSB<UGSBWindow>(WindowClass, WindowName))
	{
		OpenedWindows.Add(Window);
		Window->OnOpened(this);
		UCanvasPanelSlot* CanvasPanelSlot = RootCanvas->AddChildToCanvas(Window);
		FAnchors Anchors;
		Anchors.Minimum = { 0.5,0.5 };
		Anchors.Maximum = { 0.5,0.5 };
		CanvasPanelSlot->SetAnchors(Anchors);
		CanvasPanelSlot->SetPosition({ 0,0 });
		CanvasPanelSlot->SetAlignment({ 0.5, 0.5f });
		CanvasPanelSlot->SetAutoSize(true);
		UpdatePlayerControllMode();
		return Window;
	}
	return nullptr;
}

UGSBWindow* UGSBOverlay::ToggleWindow(UGSBWindow* Window, TSubclassOf<UGSBWindow> WindowClass, const FName& WindowName)
{
	if (IsWindowOpened(Window))
	{
		CloseWindow(Window);
		return nullptr;
	}
	else
	{
		return OpenWindow(WindowClass, WindowName);
	}
}

UGSBNumberInputDialog* UGSBOverlay::OpenNumberInputDialog(TSubclassOf<UGSBNumberInputDialog> NumberInputDialogClass, const FName& DialogName, UObject* TargetObject)
{
	if (UGSBNumberInputDialog* Dialog = Cast<UGSBNumberInputDialog>(OpenWindow(NumberInputDialogClass, DialogName)))
	{
		Dialog->SetTargetObject(TargetObject);
		return Dialog;
	}
	return nullptr;
}

UGSBNumberInputDialog* UGSBOverlay::OpenDefaultNumberInputDialog(const FName& DialogName, UObject* TargetObject)
{
	return OpenNumberInputDialog(DefaultNumberInputDialogClass, DialogName, TargetObject);
}

void UGSBOverlay::CloseWindow(UGSBWindow* Window)
{
	if (IsValid(Window))
	{
		Window->OnClosed();
	}
	OpenedWindows.Remove(Window);
	UpdatePlayerControllMode();
}

bool UGSBOverlay::IsWindowOpened(const UGSBWindow* Window) const
{
	return IsValid(Window) && OpenedWindows.Contains(Window);
}

void UGSBOverlay::CloseAllWindows()
{
	for (UGSBWindow* Window : OpenedWindows)
	{
		CloseWindow(Window);
	}
	OpenedWindows.Empty();
}

UGSBContextMenu* UGSBOverlay::OpenContextMenu(TSubclassOf<UGSBContextMenu> ContextMenuClass, const FName& ContextMenuName, UObject* ContextTarget)
{
	if (UGSBContextMenu* ContextMenu = CreateWidget_GSB<UGSBContextMenu>(ContextMenuClass, ContextMenuName))
	{
		ContextMenu->AddToViewport();
		FVector2D MousePos;
		if (GetOwningPlayer()->GetMousePosition(MousePos.X, MousePos.Y))
		{
			ContextMenu->SetPositionInViewport(MousePos);
		}
		ContextMenu->OnOpened(ContextTarget);
		return ContextMenu;
	}
	return nullptr;
}

UGSBContextMenu* UGSBOverlay::OpenDefaultContextMenu(const FName& ContextMenuName, UObject* ContextTarget)
{
	if (DefaultContextMenuClass)
	{
		return OpenContextMenu(DefaultContextMenuClass, ContextMenuName, ContextTarget);
	}
	
	TRACE_SCREEN_LOG(TEXT("DefaultContextMenuClass가 nullptr 입니다."));
	return nullptr;
}
