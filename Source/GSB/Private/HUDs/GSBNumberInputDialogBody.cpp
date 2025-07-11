// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBNumberInputDialogBody.h"
#include "Components/EditableText.h"

void UGSBNumberInputDialogBody::NativeConstruct()
{
	Super::NativeConstruct();

	NumberInputText->SetText(FText::FromString(TEXT("0")));
	NumberInputText->SetFocus();
	NumberInputText->OnTextChanged.AddDynamic(this, &UGSBNumberInputDialogBody::FilterNumericInput);
}

void UGSBNumberInputDialogBody::OnAttachedToDialog(UGSBDialog* Dialog)
{
	Super::OnAttachedToDialog(Dialog);

	NumberInputText->SetFocus();
}

void UGSBNumberInputDialogBody::SetNumberInput(int InNumber)
{
	NumberInputText->SetText(FText::FromString(FString::FromInt(InNumber)));
	Number = InNumber;
}

void UGSBNumberInputDialogBody::FilterNumericInput(const FText& Text)
{
	if (Text.IsNumeric())
	{
		Number = FCString::Atoi(*(Text.ToString()));
	}
	else if (Text.IsEmpty())
	{
		Number = 0;
	}

	NumberInputText->SetText(FText::FromString(FString::FromInt(Number)));
}
