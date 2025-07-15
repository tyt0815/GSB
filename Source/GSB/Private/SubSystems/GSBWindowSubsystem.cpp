// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystems/GSBWindowSubsystem.h"
#include "HUDs/GSBHUD.h"
#include "DebugHeader.h"

UGSBWindow* UGSBWindowSubsystem::OpenWindow(UClass* WindowClass, const FName& WindowName)
{
	return GetHUD()->OpenWindow(WindowClass, WindowName);
}

UGSBNumberInputDialog* UGSBWindowSubsystem::OpenNumberInputDialog(UClass* NumberInputDialogClass, const FName& DialogName, UObject* TargetObject)
{
	return GetHUD()->OpenNumberInputDialog(NumberInputDialogClass, DialogName, TargetObject);
}

UGSBNumberInputDialog* UGSBWindowSubsystem::OpenDefaultNumberInputDialog(const FName& DialogName, UObject* TargetObject)
{
	return GetHUD()->OpenDefaultNumberInputDialog(DialogName, TargetObject);
}

void UGSBWindowSubsystem::CloseWindow(UGSBWindow* Window)
{
	GetHUD()->CloseWindow(Window);
}

bool UGSBWindowSubsystem::IsWindowOpened(UGSBWindow* Window)
{
	return GetHUD()->IsWindowOpened(Window);
}

void UGSBWindowSubsystem::CloseAllWindows()
{
	GetHUD()->CloseAllWindows();
}

UGSBContextMenu* UGSBWindowSubsystem::OpenContextMenu(UClass* ContextMenuClass, const FName& ContextMenuName, UObject* ContextTarget)
{
	return GetHUD()->OpenContextMenu(ContextMenuClass, ContextMenuName, ContextTarget);
}

UGSBContextMenu* UGSBWindowSubsystem::OpenDefaultContextMenu(const FName& ContextMenuName, UObject* ContextTarget)
{
	return GetHUD()->OpenDefaultContextMenu(ContextMenuName, ContextTarget);
}

AGSBHUD* UGSBWindowSubsystem::GetHUD()
{
	if (!IsValid(HUD))
	{
		HUD = Cast<AGSBHUD>(GetGameInstance()->GetPrimaryPlayerController()->GetHUD());
	}
	
	return HUD;
}
