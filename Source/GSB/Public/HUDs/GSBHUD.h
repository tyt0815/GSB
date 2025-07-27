// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUDs/GSBOverlay.h"
#include "GSBHUD.generated.h"

class UGSBWindow;
class UGSBContextMenu;
class UGSBNumberInputDialog;

UCLASS()
class GSB_API AGSBHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

public:
	UGSBOverlay* AddOverlay(TSubclassOf<UGSBOverlay> OverlayClass);

	void RemoveOverlayAtTop();

	UGSBWindow* OpenWindow(UClass* WindowClass, const FName& DebugName);

	UGSBWindow* ToggleWindow(UGSBWindow* Window, UClass* WindowClass, const FName& DebugName);

	UGSBNumberInputDialog* OpenNumberInputDialog(UClass* NumberInputDialogClass, UObject* TargetObject, const FName& DebugName);

	UGSBNumberInputDialog* OpenDefaultNumberInputDialog(UObject* TargetObject, const FName& DebugName);

	UGSBOverlay* GetOverlayByWindow(UGSBWindow* Window);

	void CloseWindow(UGSBWindow* Window);

	bool IsWindowOpened(UGSBWindow* Window);

	void CloseAllWindowsOnCurrentOverlay();

	UGSBContextMenu* OpenContextMenu(UClass* ContextMenuClass, UObject* ContextTarget, const FName& DebugName);

	UGSBContextMenu* OpenDefaultContextMenu(UObject* ContextTarget, const FName& DebugName);

	UGSBOverlay* GetCurrentOverlay() const;

protected:
	UPROPERTY()
	TArray<UGSBOverlay*> Overlays;
};
