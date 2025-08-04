// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBProductionFacilityDetailWindow.h"
#include "HUDs/GSBInventory.h"
#include "HUDs/GSBItemSlot.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Interfaces/InventoryInterface.h"
#include "Facility/ProductionFacility.h"
#include "DebugHeader.h"

void UGSBProductionFacilityDetailWindow::NativeConstruct()
{
	Super::NativeConstruct();

	Inventory->OnItemSlotAdded.AddDynamic(this, &UGSBProductionFacilityDetailWindow::HandleOnInventoryItemSlotAdded);
	InputStorage->OnItemSlotAdded.AddDynamic(this, &UGSBProductionFacilityDetailWindow::HandleOnInputStorageItemSlotAdded);
	OutputStorage->OnItemSlotAdded.AddDynamic(this, &UGSBProductionFacilityDetailWindow::HandleOnOutputStorageItemSlotAdded);
}

void UGSBProductionFacilityDetailWindow::NativeTick(const FGeometry& MyGeometry, float Seconds)
{
	Super::NativeTick(MyGeometry, Seconds);

	if (AProductionFacility* ProductionFacility = Cast<AProductionFacility>(LinkedFacility))
	{
		ProductionTime->SetText(FText::FromString(FString::SanitizeFloat(ProductionFacility->GetProductionTime())));
		ProductionProgress->SetPercent(ProductionFacility->GetProductionProgress());
	}
}

void UGSBProductionFacilityDetailWindow::OnLinkedToFacility(AFacility* Facility)
{
	Super::OnLinkedToFacility(Facility);

	if (AProductionFacility* ProductionFacility = Cast<AProductionFacility>(Facility))
	{
		InputStorage->LinkStorageComponent(ProductionFacility->GetInputStorageComponent());
		OutputStorage->LinkStorageComponent(ProductionFacility->GetOutputStorageComponent());
	}

	if (IInventoryInterface* InventoryInterface = Cast<IInventoryInterface>(GetOwningPlayerPawn()))
	{
		Inventory->LinkInventoryComponent(InventoryInterface->GetInventoryComponent());
	}
}

void UGSBProductionFacilityDetailWindow::HandleOnInventoryItemSlotAdded(UGSBStorage* StorageBody, UGSBItemList* ItemList, UGSBItemSlot* ItemSlot)
{
	ItemSlot->OnItemSlotLeftClicked.AddDynamic(this, &UGSBProductionFacilityDetailWindow::HandleOnInventoryItemSlotLeftClicked);
}

void UGSBProductionFacilityDetailWindow::HandleOnInventoryItemSlotLeftClicked(UGSBItemSlot* ItemSlotWidget)
{
	Inventory->MoveAllItemTo(InputStorage, ItemSlotWidget->GetItemData());
	if (AProductionFacility* ProductionFacility = Cast<AProductionFacility>(LinkedFacility))
	{
		ProductionFacility->TryStartProduction();
	}
}

void UGSBProductionFacilityDetailWindow::HandleOnInputStorageItemSlotAdded(UGSBStorage* StorageBody, UGSBItemList* ItemList, UGSBItemSlot* ItemSlot)
{
	ItemSlot->OnItemSlotLeftClicked.AddDynamic(this, &UGSBProductionFacilityDetailWindow::HandleOnInputStorageItemSlotLeftClicked);
}

void UGSBProductionFacilityDetailWindow::HandleOnInputStorageItemSlotLeftClicked(UGSBItemSlot* ItemSlotWidget)
{
	InputStorage->MoveAllItemTo(Inventory, ItemSlotWidget->GetItemData());
	
}

void UGSBProductionFacilityDetailWindow::HandleOnOutputStorageItemSlotAdded(UGSBStorage* StorageBody, UGSBItemList* ItemList, UGSBItemSlot* ItemSlot)
{
	ItemSlot->OnItemSlotLeftClicked.AddDynamic(this, &UGSBProductionFacilityDetailWindow::HandleOnOutputStorageItemSlotLeftClicked);
}

void UGSBProductionFacilityDetailWindow::HandleOnOutputStorageItemSlotLeftClicked(UGSBItemSlot* ItemSlotWidget)
{
	OutputStorage->MoveAllItemTo(Inventory, ItemSlotWidget->GetItemData());
}
