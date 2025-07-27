// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBHUD.h"
#include "HUDs/GSBOverlay.h"
#include "DebugHeader.h"

void AGSBHUD::BeginPlay()
{
	Super::BeginPlay();
}

UGSBOverlay* AGSBHUD::AddOverlay(TSubclassOf<UGSBOverlay> OverlayClass)
{
	if (OverlayClass)
	{
		if (APlayerController* PC = GetOwningPlayerController())
		{
			if (UGSBOverlay* NewOverlay = CreateWidget<UGSBOverlay>(PC, OverlayClass))
			{
				if (UGSBOverlay* CurrentOverlay = GetCurrentOverlay())
				{
					CurrentOverlay->RemoveFromParent();
				}
				Overlays.Add(NewOverlay);
				NewOverlay->AddToViewport();
				NewOverlay->InitializeOverlay();

				return NewOverlay;
			}
		}
	}
	return nullptr;
}

void AGSBHUD::RemoveOverlayAtTop()
{
	if (Overlays.IsEmpty())
	{
		return;
	}

	Overlays.RemoveAt(Overlays.Num() - 1);

	if (UGSBOverlay* CurrentOverlay = GetCurrentOverlay())
	{
		CurrentOverlay->AddToViewport();
	}
}

UGSBWindow* AGSBHUD::OpenWindow(UClass* WindowClass, const FName& DebugName)
{
	if (UGSBOverlay* CurrentOverlay = GetCurrentOverlay())
	{
		return CurrentOverlay->OpenWindow(WindowClass, DebugName);
	}
	return nullptr;
}

UGSBWindow* AGSBHUD::ToggleWindow(UGSBWindow* Window, UClass* WindowClass, const FName& DebugName)
{
	if (UGSBOverlay* CurrentOverlay = GetCurrentOverlay())
	{
		return CurrentOverlay->ToggleWindow(Window, WindowClass, DebugName);
	}
	return nullptr;
}

UGSBNumberInputDialog* AGSBHUD::OpenNumberInputDialog(UClass* NumberInputDialogClass, UObject* TargetObject, const FName& DebugName)
{
	if (UGSBOverlay* CurrentOverlay = GetCurrentOverlay())
	{
		return CurrentOverlay->OpenNumberInputDialog(NumberInputDialogClass, TargetObject, DebugName);
	}
	return nullptr;
}

UGSBNumberInputDialog* AGSBHUD::OpenDefaultNumberInputDialog(UObject* TargetObject, const FName& DebugName)
{
	if (UGSBOverlay* CurrentOverlay = GetCurrentOverlay())
	{
		return CurrentOverlay->OpenDefaultNumberInputDialog(TargetObject, DebugName);
	}
	return nullptr;
}

UGSBOverlay* AGSBHUD::GetOverlayByWindow(UGSBWindow* Window)
{
	for (UGSBOverlay* Overlay : Overlays)
	{
		if (Overlay->IsWindowOpened(Window))
		{
			return Overlay;
		}
	}
	return nullptr;
}

void AGSBHUD::CloseWindow(UGSBWindow* Window)
{
	if (UGSBOverlay* Overlay = GetOverlayByWindow(Window))
	{
		Overlay->CloseWindow(Window);
	}
}

bool AGSBHUD::IsWindowOpened(UGSBWindow* Window)
{
	return GetOverlayByWindow(Window) != nullptr;
}

void AGSBHUD::CloseAllWindowsOnCurrentOverlay()
{
	if (UGSBOverlay* CurrentOverlay = GetCurrentOverlay())
	{
		CurrentOverlay->CloseAllWindows();
	}
}

UGSBContextMenu* AGSBHUD::OpenContextMenu(UClass* ContextMenuClass, UObject* ContextTarget, const FName& DebugName)
{
	if (UGSBOverlay* CurrentOverlay = GetCurrentOverlay())
	{
		return CurrentOverlay->OpenContextMenu(ContextMenuClass, ContextTarget, DebugName);
	}
	return nullptr;
}

UGSBContextMenu* AGSBHUD::OpenDefaultContextMenu(UObject* ContextTarget, const FName& DebugName)
{
	if (UGSBOverlay* CurrentOverlay = GetCurrentOverlay())
	{
		return CurrentOverlay->OpenDefaultContextMenu(ContextTarget, DebugName);
	}
	return nullptr;
}

UGSBOverlay* AGSBHUD::GetCurrentOverlay() const
{
	if (!Overlays.IsEmpty())
	{
		return Overlays.Last();
	}
	return nullptr;
}
