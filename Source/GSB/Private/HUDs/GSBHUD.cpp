// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBHUD.h"
#include "HUDs/GSBOverlay.h"
#include "DebugHeader.h"

void AGSBHUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		if (OverlayWidgetClass)
		{
			OverlayWidget = CreateWidget <UGSBOverlay>(GetWorld(), OverlayWidgetClass);
			if (OverlayWidget)
			{
				OverlayWidget->AddToViewport();
				OverlayWidget->InitializeOverlay();
			}
		}
		else
		{
			OverlayWidget = CreateWidget<UGSBOverlay>(GetOwningPlayerController());
			TRACE_SCREEN_LOG(TEXT("OverlayWidgetClass가 nullptr입니다."))
		}
	}

	OnEndBeginPlay.Broadcast();
}

UGSBWindow* AGSBHUD::OpenWindow(UClass* WindowClass, const FName& WindowName)
{
	return OverlayWidget->OpenWindow(WindowClass, WindowName);
}

UGSBWindow* AGSBHUD::ToggleWindow(UGSBWindow* Window, UClass* WindowClass, const FName& WindowName)
{
	return OverlayWidget->ToggleWindow(Window, WindowClass, WindowName);
}

UGSBNumberInputDialog* AGSBHUD::OpenNumberInputDialog(UClass* NumberInputDialogClass, const FName& DialogName, UObject* TargetObject)
{
	return OverlayWidget->OpenNumberInputDialog(NumberInputDialogClass, DialogName, TargetObject);
}

UGSBNumberInputDialog* AGSBHUD::OpenDefaultNumberInputDialog(const FName& DialogName, UObject* TargetObject)
{
	return OverlayWidget->OpenDefaultNumberInputDialog(DialogName, TargetObject);
}

void AGSBHUD::CloseWindow(UGSBWindow* Window)
{
	OverlayWidget->CloseWindow(Window);
}

bool AGSBHUD::IsWindowOpened(UGSBWindow* Window)
{
	return OverlayWidget->IsWindowOpened(Window);
}

void AGSBHUD::CloseAllWindows()
{
	OverlayWidget->CloseAllWindows();
}

UGSBContextMenu* AGSBHUD::OpenContextMenu(UClass* ContextMenuClass, const FName& ContextMenuName, UObject* ContextTarget)
{
	return OverlayWidget->OpenContextMenu(ContextMenuClass, ContextMenuName, ContextTarget);
}

UGSBContextMenu* AGSBHUD::OpenDefaultContextMenu(const FName& ContextMenuName, UObject* ContextTarget)
{
	return OverlayWidget->OpenDefaultContextMenu(ContextMenuName, ContextTarget);
}
