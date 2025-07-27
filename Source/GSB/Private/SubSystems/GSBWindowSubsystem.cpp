// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystems/GSBWindowSubsystem.h"
#include "GSBGameInstance.h"
#include "HUDs/GSBHUD.h"
#include "DebugHeader.h"

UGSBWindowSubsystem* UGSBWindowSubsystem::Get(UObject* WorldContext)
{
	if (UWorld* World = WorldContext->GetWorld())
	{
		if (UGameInstance* GameInst = World->GetGameInstance())
		{
			return GameInst->GetSubsystem<UGSBWindowSubsystem>();
		}
	}
	return nullptr;
}

UGSBWindow* UGSBWindowSubsystem::OpenWindow(UClass* WindowClass, const FName& DebugName)
{
	return GetHUD()->OpenWindow(WindowClass, DebugName);
}

UGSBNumberInputDialog* UGSBWindowSubsystem::OpenNumberInputDialog(UClass* NumberInputDialogClass, UObject* TargetObject, const FName& DebugName)
{
	return GetHUD()->OpenNumberInputDialog(NumberInputDialogClass, TargetObject, DebugName);
}

UGSBNumberInputDialog* UGSBWindowSubsystem::OpenDefaultNumberInputDialog(UObject* TargetObject, const FName& DebugName)
{
	return GetHUD()->OpenDefaultNumberInputDialog(TargetObject, DebugName);
}

void UGSBWindowSubsystem::CloseWindow(UGSBWindow* Window)
{
	GetHUD()->CloseWindow(Window);
}

bool UGSBWindowSubsystem::IsWindowOpened(UGSBWindow* Window)
{
	return GetHUD()->IsWindowOpened(Window);
}

void UGSBWindowSubsystem::CloseAllWindowsOnCurrentOverlay()
{
	GetHUD()->CloseAllWindowsOnCurrentOverlay();
}

UGSBContextMenu* UGSBWindowSubsystem::OpenContextMenu(UClass* ContextMenuClass, UObject* ContextTarget, const FName& DebugName)
{
	return GetHUD()->OpenContextMenu(ContextMenuClass, ContextTarget, DebugName);
}

UGSBContextMenu* UGSBWindowSubsystem::OpenDefaultContextMenu(UObject* ContextTarget, const FName& DebugName)
{
	return GetHUD()->OpenDefaultContextMenu(ContextTarget, DebugName);
}

UGSBWindow* UGSBWindowSubsystem::ToggleWindow_Internal(UGSBWindow* Window, UClass* WindowClass, const FName& DebugName)
{
	return GetHUD()->ToggleWindow(Window, WindowClass, DebugName);
}

UGSBWindow* UGSBWindowSubsystem::ToggleWindow_Internal(UGSBWindow* Window, const FName& WindowClassName, const FName& DebugName)
{
	if (UClass* Class = GetWidgetClass(WindowClassName))
	{
		return ToggleWindow_Internal(Window, Class, DebugName);
	}
	else
	{
		return nullptr;
	}
}

UClass* UGSBWindowSubsystem::GetWidgetClass(const FName& ClassName)
{
	if (UGSBGameInstance* GameInst = Cast<UGSBGameInstance>(GetGameInstance()))
	{
		return GameInst->GetUserWidgetClass(ClassName);
	}
	return nullptr;
}

AGSBHUD* UGSBWindowSubsystem::GetHUD()
{
	if (!IsValid(HUD))
	{
		HUD = Cast<AGSBHUD>(GetGameInstance()->GetPrimaryPlayerController()->GetHUD());
	}
	return HUD;
}
