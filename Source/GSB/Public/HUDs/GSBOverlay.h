// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDs/GSBWindow.h"
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

	void CloseWindow(UGSBWindow* Window);

	bool IsWindowOpened(UGSBWindow* Window);

	void CloseAllWindows();

	UGSBContextMenu* OpenContextMenu(TSubclassOf<UGSBContextMenu> ContextMenuClass, const FName& ContextMenuName, UObject* ContextTarget);

	UGSBContextMenu* OpenDefaultContextMenu(const FName& ContextMenuName, UObject* ContextTarget);

protected:
	template<typename WidgetT>
	WidgetT* CreateWidget_GSB(TSubclassOf<UUserWidget> UserWidgetClass, FName WidgetName);

	UPROPERTY(EditDefaultsOnly, Category = "Subclasses")
	TSubclassOf<UGSBContextMenu> DefaultContextMenuClass;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* RootCanvas;

	TArray<UGSBWindow*> OpenedWindows;
};

template<typename WidgetT>
inline WidgetT* UGSBOverlay::CreateWidget_GSB(TSubclassOf<UUserWidget> UserWidgetClass, FName WidgetName)
{
	if (WidgetT* Widget = CreateWidget<WidgetT>(GetOwningPlayer(), UserWidgetClass, WidgetName))
	{
		return Widget;
	}
	TRACE_SCREEN_LOG(WidgetName.ToString() + TEXT(" 위젯 생성 실패"));
	return nullptr;
}
