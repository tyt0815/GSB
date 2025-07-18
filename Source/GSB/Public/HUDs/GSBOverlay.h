// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDs/GSBWindow.h"
#include "HUDs/GSBNumberInputDialog.h"
#include "HUDs/GSBContextMenu.h"
#include "DebugHeader.h"
#include "GSBOverlay.generated.h"

class UCanvasPanel;

UCLASS()
class GSB_API UGSBOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void InitializeOverlay();

	virtual void UpdatePlayerControllMode() {};

	UGSBWindow* OpenWindow(TSubclassOf<UGSBWindow> WindowClass, const FName& WindowName);

	UGSBWindow* ToggleWindow(UGSBWindow* Window, TSubclassOf<UGSBWindow> WindowClass, const FName& WindowName);

	UGSBNumberInputDialog* OpenNumberInputDialog(TSubclassOf<UGSBNumberInputDialog> NumberInputDialogClass, const FName& DialogName, UObject* TargetObject);
	
	UGSBNumberInputDialog* OpenDefaultNumberInputDialog(const FName& DialogName, UObject* TargetObject);

	void CloseWindow(UGSBWindow* Window);

	bool IsWindowOpened(const UGSBWindow* Window) const;

	void CloseAllWindows();

	UGSBContextMenu* OpenContextMenu(TSubclassOf<UGSBContextMenu> ContextMenuClass, const FName& ContextMenuName, UObject* ContextTarget);

	UGSBContextMenu* OpenDefaultContextMenu(const FName& ContextMenuName, UObject* ContextTarget);

protected:
	template<typename WidgetT>
	WidgetT* CreateWidget_GSB(TSubclassOf<UUserWidget> UserWidgetClass, FName WidgetName);

	UPROPERTY(EditDefaultsOnly, Category = "Subclasses")
	TSubclassOf<UGSBContextMenu> DefaultContextMenuClass;

	UPROPERTY(EditDefaultsOnly, Category = "Subclasses")
	TSubclassOf<UGSBNumberInputDialog> DefaultNumberInputDialogClass;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* RootCanvas;

	TArray<UGSBWindow*> OpenedWindows;
};

template<typename WidgetT>
inline WidgetT* UGSBOverlay::CreateWidget_GSB(TSubclassOf<UUserWidget> UserWidgetClass, FName WidgetName)
{
	if (WidgetT* Widget = CreateWidget<WidgetT>(GetOwningPlayer(), UserWidgetClass, NAME_None))
	{
		return Widget;
	}
	TRACE_SCREEN_LOG(WidgetName.ToString() + TEXT(" 위젯 생성 실패"));
	return nullptr;
}
