// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBOutputPortDetailWindow.h"
#include "HUDs/GSBStorageBody.h"
#include "HUDs/GSBItemSlot.h"
#include "Facility/Addon/ConfigurableOutputPort.h"

void UGSBOutputPortDetailWindow::NativeConstruct()
{
	Super::NativeConstruct();

	HubStorage->OnItemSlotAdded.AddDynamic(this, &UGSBOutputPortDetailWindow::HandleOnItemSlotAdded);
	OutputItemSlot->OnItemSlotLeftClicked.AddDynamic(this, &UGSBOutputPortDetailWindow::HandleOnOutputItemSlotClicked);
}

void UGSBOutputPortDetailWindow::LinkOutputPort(AConfigurableOutputPort* Port)
{
	LinkedOutputPort = Port;
	if (IsValid(LinkedOutputPort))
	{
		HubStorage->LinkStorageComponent(LinkedOutputPort->LinkedStorage);
		SetOutputItem(LinkedOutputPort->SelectedItem);
	}
}

void UGSBOutputPortDetailWindow::SetOutputItem(UItemDataAsset* ItemData)
{
	OutputItemSlot->SetLabelByItemDataAsset(ItemData);
}

void UGSBOutputPortDetailWindow::HandleOnItemSlotAdded(UGSBStorageBody* StorageBody, UGSBItemList* ItemList, UGSBItemSlot* ItemSlot)
{
	ItemSlot->OnItemSlotLeftClicked.AddDynamic(this, &UGSBOutputPortDetailWindow::HandleOnItemSlotLeftClicked);
}

void UGSBOutputPortDetailWindow::HandleOnItemSlotLeftClicked(UGSBItemSlot* ItemSlot)
{
	if (IsValid(LinkedOutputPort))
	{
		LinkedOutputPort->UpdateSelectedItem(ItemSlot->GetItemData());
	}
}

void UGSBOutputPortDetailWindow::HandleOnOutputItemSlotClicked(UGSBItemSlot* ItemSlot)
{
	if (IsValid(LinkedOutputPort))
	{
		LinkedOutputPort->UpdateSelectedItem(nullptr);
	}
}
