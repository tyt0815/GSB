// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDs/GSBWindow.h"
#include "HUDs/GSBNumberInputDialog.h"
#include "HUDs/GSBContextMenu.h"
#include "DebugHeader.h"
#include "GSBOverlayPanel.generated.h"

class UCanvasPanel;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGSBWindowOpened, UGSBWindow*, Window);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGSBWindowCloseed, UGSBWindow*, Window);

UCLASS()
class GSB_API UGSBOverlayPanel : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void InitializeOverlayPanel();

	UGSBWindow* OpenWindow(TSubclassOf<UGSBWindow> WindowClass, const FName& DebugName);

	UGSBWindow* ToggleWindow(UGSBWindow* Window, TSubclassOf<UGSBWindow> WindowClass, const FName& DebugName);

	UGSBNumberInputDialog* OpenNumberInputDialog(TSubclassOf<UGSBNumberInputDialog> NumberInputDialogClass, UObject* TargetObject, const FName& DebugName);

	UGSBNumberInputDialog* OpenDefaultNumberInputDialog(UObject* TargetObject, const FName& DebugName);

	void CloseWindow(UGSBWindow* Window);

	bool IsWindowOpened(const UGSBWindow* Window) const;

	void CloseAllWindows();

	UGSBContextMenu* OpenContextMenu(TSubclassOf<UGSBContextMenu> ContextMenuClass, UObject* ContextTarget, const FName& DebugName);

	UGSBContextMenu* OpenDefaultContextMenu(UObject* ContextTarget, const FName& DebugName);

	FOnGSBWindowOpened OnWindowOpened;

	FOnGSBWindowCloseed OnWindowClosed;

protected:
	template<typename WidgetT>
	WidgetT* CreateWidget_GSB(TSubclassOf<UUserWidget> UserWidgetClass, const FName& DebugName);

	UPROPERTY(EditDefaultsOnly, Category = "GSB")
	TSubclassOf<UGSBContextMenu> DefaultContextMenuClass;

	UPROPERTY(EditDefaultsOnly, Category = "GSB")
	TSubclassOf<UGSBNumberInputDialog> DefaultNumberInputDialogClass;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* RootCanvas;

	TArray<UGSBWindow*> OpenedWindows;
};

template<typename WidgetT>
inline WidgetT* UGSBOverlayPanel::CreateWidget_GSB(TSubclassOf<UUserWidget> UserWidgetClass, const FName& DebugName)
{
	if (WidgetT* Widget = CreateWidget<WidgetT>(GetOwningPlayer(), UserWidgetClass, NAME_None))
	{
		return Widget;
	}
	TRACE_SCREEN_LOG(DebugName.ToString() + TEXT(" 위젯 생성 실패"));
	return nullptr;
}
