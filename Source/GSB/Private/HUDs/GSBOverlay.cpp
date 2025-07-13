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
		return Window;
	}
	return nullptr;
}

void UGSBOverlay::CloseWindow(UGSBWindow* Window)
{
	if (IsValid(Window))
	{
		Window->OnClosed();
	}
	OpenedWindows.Remove(Window);
}

bool UGSBOverlay::IsWindowOpened(UGSBWindow* Window)
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