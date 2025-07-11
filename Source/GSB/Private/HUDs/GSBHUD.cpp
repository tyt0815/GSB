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
			OverlayWidget = CreateWidget <UGSBOverlay>(GetOwningPlayerController(), OverlayWidgetClass);
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

UGSBWindowWidget* AGSBHUD::OpenWindow_Internal(UObject* InTargetObject)
{
	return OverlayWidget->OpenWindow_Internal(InTargetObject);
}

void AGSBHUD::CloseWindow_Internal(UGSBWindowWidget* WindowWidget)
{
	OverlayWidget->CloseWindow_Internal(WindowWidget);
}

UGSBConfirmationDialog* AGSBHUD::OpenConfirmationDialog_Internal(UGSBDialogBody* Body, UObject* InTargetObject)
{
	return OverlayWidget->OpenConfirmationDialog_Internal(Body, InTargetObject);
}

UGSBNumberInputDialogBody* AGSBHUD::OpenNumberInputDialog_Internal(UObject* InTargetObject, int32 Number)
{
	return OverlayWidget->OpenNumberInputDialog_Internal(InTargetObject, Number);
}

bool AGSBHUD::IsOpened_Internal(UGSBWindowWidget* WindowWidget) const
{
	return OverlayWidget->IsOpened_Internal(WindowWidget);
}

void AGSBHUD::CloseAllWindows_Internal()
{
	OverlayWidget->CloseAllWindows_Internal();
}

UGSBContextMenu* AGSBHUD::OpenContextMenu_Internal(UObject* InTargetObject)
{
	return OverlayWidget->OpenContextMenu_Internal(InTargetObject);
}
