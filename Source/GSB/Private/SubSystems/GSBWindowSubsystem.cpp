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

UGSBWindow* UGSBWindowSubsystem::ToggleWindow_Internal(UGSBWindow* Window, UClass* WindowClass, const FName& WindowName)
{
	return GetHUD()->ToggleWindow(Window, WindowClass, WindowName);
}

UGSBWindow* UGSBWindowSubsystem::ToggleWindow_Internal(UGSBWindow* Window, const FName& WindowClassName, const FName& WindowName)
{
	if (UClass* Class = GetWidgetClass(WindowClassName))
	{
		return ToggleWindow_Internal(Window, Class, WindowName);
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
