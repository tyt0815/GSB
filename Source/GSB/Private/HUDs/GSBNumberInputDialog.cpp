// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBNumberInputDialog.h"
#include "Components/Button.h"
#include "Components/EditableText.h"

void UGSBNumberInputDialog::NativeConstruct()
{
	NumberInputText->SetText(FText::FromString(TEXT("0")));
	NumberInputText->SetFocus();
	NumberInputText->OnTextChanged.AddDynamic(this, &UGSBNumberInputDialog::FilterNumericInput);

	OKButton->OnClicked.AddDynamic(this, &UGSBNumberInputDialog::HandleOnOKButtonClicked);
	CancelButton->OnClicked.AddDynamic(this, &UGSBWindow::Close);
}

void UGSBNumberInputDialog::SetNumberInput(int InNumber)
{
	NumberInputText->SetText(FText::FromString(FString::FromInt(InNumber)));
	Number = InNumber;
}

void UGSBNumberInputDialog::FilterNumericInput(const FText& Text)
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

void UGSBNumberInputDialog::HandleOnOKButtonClicked()
{
	OnOKButtonClicked.Broadcast(this);
	Close();
}
