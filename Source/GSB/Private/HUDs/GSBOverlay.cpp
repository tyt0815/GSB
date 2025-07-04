// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBOverlay.h"
#include "HUDs/GSBWindowWidget.h"
#include "HUDs/GSBConfirmationDialog.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "DebugHeader.h"

void UGSBOverlay::InitializeOverlay()
{
}

UGSBWindowWidget* UGSBOverlay::OpenWindow()
{
	if (WindowWidgetClass)
	{
		if (UGSBWindowWidget* WindowWidget = CreateWidget<UGSBWindowWidget>(GetOwningPlayer(), WindowWidgetClass))
		{
			WindowWidgets.Add(WindowWidget);
			WindowWidget->OnOpenWindow(this);
			UCanvasPanelSlot* CanvasPanelSlot = RootCanvas->AddChildToCanvas(WindowWidget);
			FAnchors Anchors;
			Anchors.Minimum = { 0.5,0.5 };
			Anchors.Maximum = { 0.5,0.5 };
			CanvasPanelSlot->SetAnchors(Anchors);
			CanvasPanelSlot->SetPosition({ 0,0 });
			CanvasPanelSlot->SetAlignment({ 0.5, 0.5f });
			CanvasPanelSlot->SetAutoSize(true);
			return WindowWidget;
		}
	}
	else
	{
		SCREEN_LOG_NONE_KEY(TEXT("UGSBOverlay::OpenWindow: WindowWidgetClass가 nullptr입니다."));
	}
	return nullptr;
}

UGSBWindowWidget* UGSBOverlay::OpenWindow(UGSBWindowBody* WindowBody)
{
	if (IsValid(WindowBody))
	{
		if (UGSBWindowWidget* WindowWidget = OpenWindow())
		{
			WindowWidget->AttachWindowBody(WindowBody);
			return WindowWidget;
		}
	}
	
	return nullptr;
}

void UGSBOverlay::CloseWindow(UGSBWindowWidget* WindowWidget)
{
	if (IsValid(WindowWidget))
	{
		WindowWidget->OnCloseWindow();
	}
	WindowWidgets.Remove(WindowWidget);
}

UGSBConfirmationDialog* UGSBOverlay::OpenConfirmationDialog()
{
	
	if (ConfirmationDialogWidgetClass)
	{
		if (UGSBConfirmationDialog* Dialog = CreateWidget<UGSBConfirmationDialog>(GetOwningPlayer(), ConfirmationDialogWidgetClass))
		{
			if (UGSBWindowWidget* Window = OpenWindow(Dialog))
			{
				Dialog->SetParentWindow(Window);
				return Dialog;
			}
		}
	}
	else
	{
		TRACE_SCREEN_LOG(TEXT("ConfirmationDialogWidgetClass가 nullptr입니다"));
	}
	
	return nullptr;
}

bool UGSBOverlay::IsOpened(UGSBWindowWidget* WindowWidget) const
{
	return WindowWidgets.Contains(WindowWidget);
}

void UGSBOverlay::CloseAllWindows()
{
	for (int i = WindowWidgets.Num() - 1; i >= 0; --i)
	{
		CloseWindow(WindowWidgets[i]);
	}
}