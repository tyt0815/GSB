// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBPowerGeneratorDetailWindow.h"
#include "HUDs/GSBInventory.h"
#include "HUDs/GSBItemSlot.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Interfaces/InventoryInterface.h"
#include "Facility/PowerGenerator.h"
#include "Items/ItemDataAsset.h"

void UGSBPowerGeneratorDetailWindow::NativeConstruct()
{
	Super::NativeConstruct();

	Inventory->OnItemSlotAdded.AddDynamic(this, &UGSBPowerGeneratorDetailWindow::HandleOnInventoryItemSlotAdded);
	ConsumingItemStorage->OnItemSlotAdded.AddDynamic(this, &UGSBPowerGeneratorDetailWindow::HandleOnConsumingItemStorageItemSlotAdded);
}

void UGSBPowerGeneratorDetailWindow::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (APowerGenerator* PowerGenerator = Cast<APowerGenerator>(LinkedFacility))
	{
		SetRemainingPowerTimeProgressBar(PowerGenerator->GetRemainingPowerTimeProgress());
	}
}

void UGSBPowerGeneratorDetailWindow::OnLinkedToFacility(AFacility* Facility)
{
	Super::OnLinkedToFacility(Facility);

	if (APowerGenerator* PowerGenerator = Cast<APowerGenerator>(LinkedFacility))
	{
		SetConsumingItem(PowerGenerator->ConsumingItemData->Name);
		SetGeneratingTime(PowerGenerator->GeneratingTime);
		ConsumingItemStorage->LinkStorageComponent(PowerGenerator->ItemStorageComponent);
	}

	if (IInventoryInterface* InventoryInterface = Cast<IInventoryInterface>(GetOwningPlayerPawn()))
	{
		Inventory->LinkInventoryComponent(InventoryInterface->GetInventoryComponent());
	}
}

void UGSBPowerGeneratorDetailWindow::SetConsumingItem(const FText& ConsumingItemName)
{
	ConsumingItem->SetText(ConsumingItemName);
}

void UGSBPowerGeneratorDetailWindow::SetGeneratingTime(float Seconds)
{
	GeneratingTime->SetText(FText::FromString(FString::SanitizeFloat(Seconds)));
}

void UGSBPowerGeneratorDetailWindow::SetRemainingPowerTimeProgressBar(float Progress)
{
	RemainingPowerTimeProgressBar->SetPercent(Progress);
}

void UGSBPowerGeneratorDetailWindow::HandleOnInventoryItemSlotAdded(UGSBStorage* StorageBody, UGSBItemList* ItemList, UGSBItemSlot* ItemSlot)
{
	ItemSlot->OnItemSlotLeftClicked.AddDynamic(this, &UGSBPowerGeneratorDetailWindow::HandleOnInventoryItemSlotLeftClicked);
}

void UGSBPowerGeneratorDetailWindow::HandleOnInventoryItemSlotLeftClicked(UGSBItemSlot* ItemSlot)
{
	if (APowerGenerator* PowerGenerator = Cast<APowerGenerator>(LinkedFacility))
	{
		if (ItemSlot->GetItemData() == PowerGenerator->ConsumingItemData)
		{
			Inventory->MoveAllItemTo(ConsumingItemStorage, PowerGenerator->ConsumingItemData);
			PowerGenerator->TryBeginGeneraingPower();
		}
	}
}

void UGSBPowerGeneratorDetailWindow::HandleOnConsumingItemStorageItemSlotAdded(UGSBStorage* StorageBody, UGSBItemList* ItemList, UGSBItemSlot* ItemSlot)
{
	ItemSlot->OnItemSlotLeftClicked.AddDynamic(this, &UGSBPowerGeneratorDetailWindow::HandleOnConsumingItemStorageItemSlotLeftClicked);
}

void UGSBPowerGeneratorDetailWindow::HandleOnConsumingItemStorageItemSlotLeftClicked(UGSBItemSlot* ItemSlot)
{
	ConsumingItemStorage->MoveAllItemTo(Inventory, ItemSlot->GetItemData());
}
