// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GSBHUD.generated.h"

class UGSBOverlay;
class UGSBWindow;
class UGSBContextMenu;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndBeginPlaySignature);

UCLASS()
class GSB_API AGSBHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

public:
	UGSBWindow* OpenWindow(UClass* WindowClass, const FName& WindowName);

	void CloseWindow(UGSBWindow* Window);

	bool IsWindowOpened(UGSBWindow* Window);

	void CloseAllWindows();

	UGSBContextMenu* OpenContextMenu(UClass* ContextMenuClass, const FName& ContextMenuName, UObject* ContextTarget);

	UGSBContextMenu* OpenDefaultContextMenu(const FName& ContextMenuName, UObject* ContextTarget);

	FOnEndBeginPlaySignature OnEndBeginPlay;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "AGSBHUD")
	TSubclassOf<UGSBOverlay> OverlayWidgetClass;

	UPROPERTY()
	UGSBOverlay* OverlayWidget;

public:
	FORCEINLINE UGSBOverlay* GetOverlayWidget() const
	{
		return OverlayWidget;
	}	
};
