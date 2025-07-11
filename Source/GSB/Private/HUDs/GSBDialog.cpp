// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBDialog.h"
#include "HUDs/GSBDialogBody.h"
#include "Components/NamedSlot.h"

void UGSBDialog::NativeOnFocusChanging(const FWeakWidgetPath& PreviousFocusPath, const FWidgetPath& NewWidgetPath, const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusChanging(PreviousFocusPath, NewWidgetPath, InFocusEvent);

	if (IsLostFocus(NewWidgetPath))
	{
		CloseWindow();
	}
}

void UGSBDialog::AttachDialogBody(UGSBDialogBody* DialogBody)
{
	DialogBodySlot->ClearChildren();
	AttachedDialogBody = DialogBody;
	if (IsValid(AttachedDialogBody))
	{
		DialogBodySlot->AddChild(AttachedDialogBody);
		AttachedDialogBody->OnAttachedToDialog(this);
	}
}

UGSBDialogBody* UGSBDialog::GetDialogBody() const
{
	return Cast<UGSBDialogBody>(DialogBodySlot->GetChildAt(0));
}
