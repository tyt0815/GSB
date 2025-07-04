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
			SCREEN_LOG_NONE_KEY(TEXT("AGSBHUD::BeginPlay: UGSBOverlay 클래스가 설정되지 않았습니다."))
		}
	}
}

UGSBWindowWidget* AGSBHUD::OpenWindow(UGSBWindowBody* InWindowBody)
{
	return OverlayWidget->OpenWindow(InWindowBody);
}

void AGSBHUD::CloseWindow(UGSBWindowWidget* WindowWidget)
{
	OverlayWidget->CloseWindow(WindowWidget);
}

UGSBConfirmationDialog* AGSBHUD::OpenConfirmationDialog()
{
	return OverlayWidget->OpenConfirmationDialog();
}

bool AGSBHUD::IsOpened(UGSBWindowWidget* WindowWidget) const
{
	return OverlayWidget->IsOpened(WindowWidget);
}

void AGSBHUD::CloseAllWindows()
{
	OverlayWidget->CloseAllWindows();
}
