// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBConfirmationDialog.h"
#include "HUDs/GSBWindowWidget.h"
#include "Components/NamedSlot.h"
#include "Components/Button.h"

void UGSBConfirmationDialog::NativeConstruct()
{
	Super::NativeConstruct();

	OKButton->OnClicked.AddDynamic(this, &UGSBConfirmationDialog::HandleOnOkButtonClicked);
	CancelButton->OnClicked.AddDynamic(this, &UGSBConfirmationDialog::CloseDialog);
}

void UGSBConfirmationDialog::NativeOnFocusChanging(const FWeakWidgetPath& PreviousFocusPath, const FWidgetPath& NewWidgetPath, const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusChanging(PreviousFocusPath, NewWidgetPath, InFocusEvent);

	if (IsLostFocus(NewWidgetPath))
	{
		CloseDialog();
	}
}

void UGSBConfirmationDialog::UpdateContent(UWidget* Content)
{
	ContentSlot->ClearChildren();
	ContentSlot->AddChild(Content);
}

void UGSBConfirmationDialog::CloseDialog()
{
	if (IsValid(ParentWindow))
	{
		ParentWindow->CloseWindow();
	}
	else
	{
		RemoveFromParent();
	}
}

bool UGSBConfirmationDialog::IsLostFocus(const FWidgetPath& NewWidgetPath)
{
	return (IsValid(ParentWindow) && !NewWidgetPath.ContainsWidget(&ParentWindow->TakeWidget().Get())) ||
		!NewWidgetPath.ContainsWidget(&TakeWidget().Get());
}

void UGSBConfirmationDialog::HandleOnOkButtonClicked()
{
	OnOKButtonClicked.Broadcast(this);
}
