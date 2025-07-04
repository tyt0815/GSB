// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBWindowBody.h"
#include "GSBConfirmationDialog.generated.h"

class UButton;
class UNamedSlot;
class UGSBConfirmationDialog;
class UGSBWindowWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOKButtonClickedSignature, UGSBConfirmationDialog*, ConfirmationDialog);

UCLASS()
class GSB_API UGSBConfirmationDialog : public UGSBWindowBody
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	virtual void NativeOnFocusChanging(const FWeakWidgetPath& PreviousFocusPath, const FWidgetPath& NewWidgetPath, const FFocusEvent& InFocusEvent) override;

public:
	void UpdateContent(UWidget* Content);

	void CloseDialog();

	FOnOKButtonClickedSignature OnOKButtonClicked;

	UGSBWindowWidget* ParentWindow;
	
private:
	UPROPERTY(meta = (BindWidget))
	UNamedSlot* ContentSlot;

	UPROPERTY(meta = (BindWidget))
	UButton* OKButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CancelButton;

	bool IsLostFocus(const FWidgetPath& NewWidgetPath);

	UFUNCTION()
	void HandleOnOkButtonClicked();

public:
	FORCEINLINE void SetParentWindow(UGSBWindowWidget* Window)
	{
		ParentWindow = Window;
	}
};
