// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBDialogBody.h"
#include "GSBNumberInputDialogBody.generated.h"

class UEditableText;

UCLASS()
class GSB_API UGSBNumberInputDialogBody : public UGSBDialogBody
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	void OnAttachedToDialog(UGSBDialog* Dialog) override;

public:
	void SetNumberInput(int InNumber);
	
private:
	UPROPERTY(meta = (BindWidget))
	UEditableText* NumberInputText;

	int32 Number = 0;

	UFUNCTION()
	void FilterNumericInput(const FText& Text);

public:
	FORCEINLINE int32 GetNumber() const
	{
		return Number;
	}
};
