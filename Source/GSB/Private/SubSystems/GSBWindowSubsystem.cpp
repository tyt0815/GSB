// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystems/GSBWindowSubsystem.h"
#include "HUDs/GSBHUD.h"
#include "HUDs/GSBWindowWidget.h"
#include "HUDs/GSBWindowHead.h"	
#include "HUDs/GSBWindowBody.h"
#include "DebugHeader.h"

UGSBWindowWidget* UGSBWindowSubsystem::OpenWindow(UObject* InTargetObject,UGSBWindowBody* WindowBody)
{
	if (AGSBHUD* HUD = GetHUD())
	{
		UGSBWindowWidget* Window = HUD->OpenWindow_Internal(InTargetObject);
		if (IsValid(WindowBody))
		{
			Window->AttachWindowBody(WindowBody);
		}
		return Window;
	}
	return nullptr;
}

void UGSBWindowSubsystem::CloseWindow(UGSBWindowWidget* WindowWidget)
{
	if (AGSBHUD* HUD = GetHUD())
	{
		HUD->CloseWindow_Internal(WindowWidget);
	}
}

void UGSBWindowSubsystem::CloseAllWindows()
{
	if (AGSBHUD* HUD = GetHUD())
	{
		HUD->CloseAllWindows_Internal();
	}
}

UGSBConfirmationDialog* UGSBWindowSubsystem::OpenConfirmationDialog(UGSBDialogBody* DialogBody, UObject* InTargetObject)
{
	if (AGSBHUD* HUD = GetHUD())
	{
		return HUD->OpenConfirmationDialog_Internal(DialogBody, InTargetObject);
	}
	return nullptr;
}

UGSBNumberInputDialogBody* UGSBWindowSubsystem::OpenNumberInputDialog(UObject* InTargetObject, int32 Number)
{
	if (AGSBHUD* HUD = GetHUD())
	{
		return HUD->OpenNumberInputDialog_Internal(InTargetObject, Number);
	}
	return nullptr;
}

bool UGSBWindowSubsystem::IsOpened(UGSBWindowWidget* WindowWidget)
{
	if (AGSBHUD* HUD = GetHUD())
	{
		return HUD->IsOpened_Internal(WindowWidget);
	}
	return false;
}

UGSBContextMenu* UGSBWindowSubsystem::OpenContextMenu(UObject* InTargetObject)
{
	if (AGSBHUD* HUD = GetHUD())
	{
		return HUD->OpenContextMenu_Internal(InTargetObject);
	}
	return nullptr;
}

AGSBHUD* UGSBWindowSubsystem::GetHUD() const
{
	if (AGSBHUD* HUD = Cast<AGSBHUD>(GetGameInstance()->GetPrimaryPlayerController()->GetHUD()))
	{
		return HUD;
	}
	SCREEN_LOG_NONE_KEY(TEXT("UGSBWindowSubsystem::GetHUD: AGSBHUD 캐스팅 실패"));
	return 0;
}
