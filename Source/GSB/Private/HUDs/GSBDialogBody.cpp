// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBDialogBody.h"
#include "HUDs/GSBDialog.h"

void UGSBDialogBody::OnAttachedToDialog(UGSBDialog* Dialog)
{
	ParentDialog = Dialog;
	SetTargetObject(ParentDialog->GetTargetObject());
}
