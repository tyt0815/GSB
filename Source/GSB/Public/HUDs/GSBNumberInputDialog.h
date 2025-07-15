// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBWindow.h"
#include "GSBNumberInputDialog.generated.h"

class UButton;
class UGSBNumberInputDialog;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNumberInputDialogOKButtonClickedSignature, UGSBNumberInputDialog*, NumberInputDialog);

UCLASS()
class GSB_API UGSBNumberInputDialog : public UGSBWindow
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

public:
	void SetNumberInput(int InNumber);

	FOnNumberInputDialogOKButtonClickedSignature OnOKButtonClicked;

private:
	UPROPERTY(meta = (BindWidget))
	class UEditableText* NumberInputText;

	UPROPERTY(meta = (BindWidget))
	UButton* OKButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CancelButton;

	int32 Number;

	UFUNCTION()
	void FilterNumericInput(const FText& Text);

	UFUNCTION()
	void HandleOnOKButtonClicked();

	UObject* TargetObject;

public:
	FORCEINLINE int32 GetNumber() const
	{
		return Number;
	}
	FORCEINLINE UObject* GetTargetObject() const
	{
		return TargetObject;
	}
	FORCEINLINE void SetTargetObject(UObject* Object)
	{
		TargetObject = Object;
	}
};
