// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBConfirmationDialog.h"
#include "HUDs/GSBWindowWidget.h"
#include "Components/NamedSlot.h"
#include "Components/Button.h"

void UGSBConfirmationDialog::NativeConstruct()
{
	Super::NativeConstruct();

	OKButton->OnClicked.AddDynamic(this, &UGSBConfirmationDialog::HandleOnOkButtonClicked);
	OKButton->OnClicked.AddDynamic(this, &UGSBConfirmationDialog::CloseWindow);
	CancelButton->OnClicked.AddDynamic(this, &UGSBConfirmationDialog::CloseWindow);
}

void UGSBConfirmationDialog::HandleOnOkButtonClicked()
{
	OnOKButtonClicked.Broadcast(this);
}
