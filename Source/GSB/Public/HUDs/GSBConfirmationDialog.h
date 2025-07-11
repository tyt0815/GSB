// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBDialog.h"
#include "GSBConfirmationDialog.generated.h"

class UButton;
class UNamedSlot;
class UGSBConfirmationDialog;
class UGSBWindowWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOKButtonClickedSignature, UGSBConfirmationDialog*, ConfirmationDialog);

UCLASS()
class GSB_API UGSBConfirmationDialog : public UGSBDialog
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

public:
	FOnOKButtonClickedSignature OnOKButtonClicked;
	
private:
	UPROPERTY(meta = (BindWidget))
	UButton* OKButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CancelButton;

	UFUNCTION()
	void HandleOnOkButtonClicked();
};
