// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystems/GSBWindowSubsystem.h"
#include "HUDs/GSBHUD.h"
#include "DebugHeader.h"

UGSBWindowWidget* UGSBWindowSubsystem::OpenWindow(UGSBWindowBody* WindowBody)
{
	if (AGSBHUD* HUD = GetHUD())
	{
		return HUD->OpenWindow(WindowBody);
	}
	return nullptr;
}

void UGSBWindowSubsystem::CloseWindow(UGSBWindowWidget* WindowWidget)
{
	if (AGSBHUD* HUD = GetHUD())
	{
		HUD->CloseWindow(WindowWidget);
	}
}

UGSBConfirmationDialog* UGSBWindowSubsystem::OpenConfirmationDialog()
{
	if (AGSBHUD* HUD = GetHUD())
	{
		return HUD->OpenConfirmationDialog();
	}
	return nullptr;
}

bool UGSBWindowSubsystem::IsOpened(UGSBWindowWidget* WindowWidget)
{
	if (AGSBHUD* HUD = GetHUD())
	{
		return HUD->IsOpened(WindowWidget);
	}
	return false;
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
