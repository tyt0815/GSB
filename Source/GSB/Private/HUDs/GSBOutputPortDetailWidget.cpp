// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBOutputPortDetailWidget.h"
#include "HUDs/GSBStorageWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Facility/Addon/ConfigurableOutputPort.h"

void UGSBOutputPortDetailWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Storage->OnItemSlotLeftClicked.AddDynamic(this, &UGSBOutputPortDetailWidget::HandleStorageItemSlotLeftClicked);
	Storage->SetTitle(FText::FromString(TEXT("허브 저장소")));
	SelectedItemSlot->OnItemSlotClickedDelegates.OnItemSlotLeftClicked.AddDynamic(this, &UGSBOutputPortDetailWidget::ClearSelectedItem);
}

bool UGSBOutputPortDetailWidget::TryLinkItemStorageComponent(UItemStorageComponent* ItemStorage)
{
	return Storage->TryLinkStorageComponent(ItemStorage);
}

void UGSBOutputPortDetailWidget::SetSelectedItem(UItemDataAsset* ItemData)
{
	FItemStack ItemStack = {};
	ItemStack.ItemData = ItemData;
	ItemStack.Stack = 0;
	SelectedItemSlot->UpdateItemData(ItemStack);
}

void UGSBOutputPortDetailWidget::UpdateStorageWidget()
{
	Storage->UpdateWidget();
}

void UGSBOutputPortDetailWidget::HandleStorageItemSlotLeftClicked(UGSBItemSlotWidget* ItemSlotWidget)
{
	if (IsValid(LinkedOutputPort))
	{
		LinkedOutputPort->UpdateSelectedItem(ItemSlotWidget->GetItemData());
	}
}

void UGSBOutputPortDetailWidget::ClearSelectedItem(UGSBItemSlotWidget* ItemSlotWidget)
{
	if (IsValid(LinkedOutputPort))
	{
		LinkedOutputPort->UpdateSelectedItem(nullptr);
	}
}
