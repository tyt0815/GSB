// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBOverlayPanel.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

void UGSBOverlayPanel::InitializeOverlayPanel()
{
}

UGSBWindow* UGSBOverlayPanel::OpenWindow(TSubclassOf<UGSBWindow> WindowClass, const FName& DebugName)
{
	if (UGSBWindow* Window = CreateWidget_GSB<UGSBWindow>(WindowClass, DebugName))
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
		OnWindowOpened.Broadcast(Window);
		return Window;
	}
	return nullptr;
}

UGSBWindow* UGSBOverlayPanel::ToggleWindow(UGSBWindow* Window, TSubclassOf<UGSBWindow> WindowClass, const FName& DebugName)
{
	if (IsWindowOpened(Window))
	{
		CloseWindow(Window);
		return nullptr;
	}
	else
	{
		return OpenWindow(WindowClass, DebugName);
	}
}

UGSBNumberInputDialog* UGSBOverlayPanel::OpenNumberInputDialog(TSubclassOf<UGSBNumberInputDialog> NumberInputDialogClass, UObject* TargetObject, const FName& DebugName)
{
	if (UGSBNumberInputDialog* Dialog = Cast<UGSBNumberInputDialog>(OpenWindow(NumberInputDialogClass, DebugName)))
	{
		Dialog->SetTargetObject(TargetObject);
		return Dialog;
	}
	return nullptr;
}

UGSBNumberInputDialog* UGSBOverlayPanel::OpenDefaultNumberInputDialog(UObject* TargetObject, const FName& DebugName)
{
	return OpenNumberInputDialog(DefaultNumberInputDialogClass, TargetObject, DebugName);
}

void UGSBOverlayPanel::CloseWindow(UGSBWindow* Window)
{
	OpenedWindows.Remove(Window);
	if (IsValid(Window))
	{
		OnWindowClosed.Broadcast(Window);
		Window->OnClosed();
	}
}

bool UGSBOverlayPanel::IsWindowOpened(const UGSBWindow* Window) const
{
	return IsValid(Window) && OpenedWindows.Contains(Window);
}

void UGSBOverlayPanel::CloseAllWindows()
{
	for (UGSBWindow* Window : OpenedWindows)
	{
		CloseWindow(Window);
	}
	OpenedWindows.Empty();
}

UGSBContextMenu* UGSBOverlayPanel::OpenContextMenu(TSubclassOf<UGSBContextMenu> ContextMenuClass, UObject* ContextTarget, const FName& DebugName)
{
	if (UGSBContextMenu* ContextMenu = CreateWidget_GSB<UGSBContextMenu>(ContextMenuClass, DebugName))
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

UGSBContextMenu* UGSBOverlayPanel::OpenDefaultContextMenu(UObject* ContextTarget, const FName& DebugName)
{
	if (DefaultContextMenuClass)
	{
		return OpenContextMenu(DefaultContextMenuClass, ContextTarget, DebugName);
	}

	TRACE_SCREEN_LOG(TEXT("DefaultContextMenuClass가 nullptr 입니다."));
	return nullptr;
}
