// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBOverlay.h"
#include "HUDs/GSBWindowWidget.h"
#include "HUDs/GSBConfirmationDialog.h"
#include "HUDs/GSBNumberInputDialogBody.h"
#include "HUDs/GSBContextMenu.h"

#include "Components/CanvasPanelSlot.h"
#include "DebugHeader.h"

void UGSBOverlay::InitializeOverlay()
{
}

UGSBWindowWidget* UGSBOverlay::OpenWindow_Internal(UObject* InTargetObject)
{
	if (WindowWidgetClass)
	{
		if (UGSBWindowWidget* WindowWidget = CreateWidget<UGSBWindowWidget>(GetOwningPlayer(), WindowWidgetClass))
		{
			WindowWidgets.Add(WindowWidget);
			WindowWidget->OnOpenWindow(this, InTargetObject);
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
		TRACE_SCREEN_LOG(TEXT("WindowWidgetClass가 nullptr입니다."));
	}
	return nullptr;
}

void UGSBOverlay::CloseWindow_Internal(UGSBWindowWidget* WindowWidget)
{
	if (IsValid(WindowWidget))
	{
		WindowWidget->OnCloseWindow();
	}
	WindowWidgets.Remove(WindowWidget);
}

bool UGSBOverlay::IsOpened_Internal(UGSBWindowWidget* WindowWidget) const
{
	return WindowWidgets.Contains(WindowWidget);
}

void UGSBOverlay::CloseAllWindows_Internal()
{
	for (int i = WindowWidgets.Num() - 1; i >= 0; --i)
	{
		CloseWindow_Internal(WindowWidgets[i]);
	}
}

UGSBConfirmationDialog* UGSBOverlay::OpenConfirmationDialog_Internal(UGSBDialogBody* DialogBody, UObject* InTargetObject)
{
	if (IsValid(DialogBody))
	{
		if (UGSBConfirmationDialog* Dialog = OpenConfirmationDialog_Internal(InTargetObject))
		{
			Dialog->AttachDialogBody(DialogBody);
			return Dialog;
		}
	}
	return nullptr;
}

UGSBNumberInputDialogBody* UGSBOverlay::OpenNumberInputDialog_Internal(UObject* InTargetObject, int Number)
{
	if (NumberInputDialogBodyWidgetClass)
	{
		if (UGSBNumberInputDialogBody* Body = CreateWidget<UGSBNumberInputDialogBody>(GetOwningPlayer(), NumberInputDialogBodyWidgetClass))
		{
			if (UGSBConfirmationDialog* Dialog = OpenConfirmationDialog_Internal(Body, InTargetObject))
			{
				Body->SetNumberInput(Number);
				return Body;
			}
		}
	}
	else
	{
		TRACE_SCREEN_LOG(TEXT("NumberInputDialogBodyWidgetClass가 nullptr 입니다."));
	}
	
	return nullptr;
}

UGSBContextMenu* UGSBOverlay::OpenContextMenu_Internal(UObject* ContextTarget)
{
	if (ContextMenuWidgetClass)
	{
		if (UGSBContextMenu* Menu = CreateWidget<UGSBContextMenu>(GetOwningPlayer(), ContextMenuWidgetClass))
		{
			Menu->AddToViewport();
			FVector2D MousePos;
			if (GetOwningPlayer()->GetMousePosition(MousePos.X, MousePos.Y))
			{
				Menu->SetPositionInViewport(MousePos);
			}

			Menu->SetFocus();
			Menu->SetContextTarget(ContextTarget);
			return Menu;
		}
	}
	else
	{
		TRACE_SCREEN_LOG(TEXT("ContextMenuWidgetClass가 nullptr 입니다."))
	}
	return nullptr;
}

UGSBConfirmationDialog* UGSBOverlay::OpenConfirmationDialog_Internal(UObject* InTargetObject)
{
	if (ConfirmationDialogWidgetClass)
	{
		if (UGSBConfirmationDialog* Dialog = CreateWidget<UGSBConfirmationDialog>(GetOwningPlayer(), ConfirmationDialogWidgetClass))
		{
			if (UGSBWindowWidget* Window = OpenWindow_Internal(InTargetObject))
			{
				Window->AttachWindowBody(Dialog);
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