// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBHubDetailView.h"
#include "HUDs/GSBStorageWidget.h"
#include "HUDs/GSBInventoryWidget.h"
#include "HUDs/GSBContextMenuEntry.h"
#include "HUDs/GSBConfirmationDialog.h"
#include "HUDs/GSBNumberInputDialogBody.h"
#include "SubSystems/GSBWindowSubsystem.h"

//void UGSBHubDetailView::NativeConstruct()
//{
//	Super::NativeConstruct();
//
//	HubStorage->SetTitle(FText::FromString(TEXT("허브 저장소")));
//	Inventory->SetTitle(FText::FromString(TEXT("인벤토리")));
//
//	HubStorage->OnItemSlotLeftClicked.AddDynamic(this, &UGSBHubDetailView::MoveItemFromHubStorageToInventory);
//	Inventory->OnItemSlotLeftClicked.AddDynamic(this, &UGSBHubDetailView::MoveItemFromInventoryToHubStorage);
//	HubStorage->OnStorageContextWidgetCreated.AddDynamic(this, &UGSBHubDetailView::HandleHubStorageContextMenuCreated);;
//	Inventory->OnStorageContextWidgetCreated.AddDynamic(this, &UGSBHubDetailView::HandleInventoryContextMenuCreated);;
//}
//
//void UGSBHubDetailView::SetHubStorageAndInventory(UItemStorageComponent* HubStorageComponent, UItemStorageComponent* InventoryComponent)
//{
//	HubStorage->TryLinkStorageComponent(HubStorageComponent);
//	Inventory->TryLinkStorageComponent(InventoryComponent);
//}
//
//void UGSBHubDetailView::MoveItemFromHubStorageToInventory(UGSBItemSlotWidget* ItemSlotWidget)
//{
//	if (IsValid(HubStorage) && IsValid(ItemSlotWidget))
//	{
//		HubStorage->MoveAllItemToStorage(Inventory, ItemSlotWidget->GetItemData());
//	}
//}
//
//void UGSBHubDetailView::MoveItemFromInventoryToHubStorage(UGSBItemSlotWidget* ItemSlotWidget)
//{
//	if (IsValid(Inventory) && IsValid(ItemSlotWidget))
//	{
//		Inventory->MoveAllItemToStorage(HubStorage, ItemSlotWidget->GetItemData());
//	}
//}
//
//void UGSBHubDetailView::HandleHubStorageContextMenuCreated(UGSBContextMenu* ContextMenuWidget)
//{
//	if (FGSBContextMenuEntryOnClickedSignature* Signature = ContextMenuWidget->AddContextMenuEntry(TEXT("옮기기")))
//	{
//		Signature->AddDynamic(this, &UGSBHubDetailView::HandleContextMenuEntry_MoveItemToInventory);
//	}
//}
//
//void UGSBHubDetailView::HandleContextMenuEntry_MoveItemToInventory(UGSBContextMenuEntry* MenuEntry)
//{
//	//UGSBWindowSubsystem* WindowManager = GetGameInstance()->GetSubsystem<UGSBWindowSubsystem>();
//	//check(WindowManager);
//	//if (UGSBNumberInputDialogBody* DialogBody = WindowManager->OpenNumberInputDialog(MenuEntry->GetTargetObject(), 0))
//	//{
//	//	if (UGSBConfirmationDialog* Dialog = DialogBody->GetParentDialog<UGSBConfirmationDialog>())
//	//	{
//	//		Dialog->OnOKButtonClicked.AddDynamic(this, &UGSBHubDetailView::HandleOnDialogOKButtonClicked_MoveItemToInventory);
//	//	}
//	//}
//
//	//MenuEntry->GetParentContextMenu()->CloseContextMenu();
//}
//
//void UGSBHubDetailView::HandleOnDialogOKButtonClicked_MoveItemToInventory(UGSBConfirmationDialog* ConfirmationDialog)
//{
//	if (UGSBNumberInputDialogBody* DialogBody = ConfirmationDialog->GetDialogBody<UGSBNumberInputDialogBody>())
//	{
//		if (UGSBItemSlotWidget* ItemSlotWidget = Cast<UGSBItemSlotWidget>(DialogBody->GetTargetObject()))
//		{
//			FItemStack ItemStack;
//			ItemStack.ItemData = ItemSlotWidget->GetItemData();
//			ItemStack.Stack = DialogBody->GetNumber();
//			HubStorage->MoveItemToStorage(Inventory, ItemStack);
//		}
//	}
//}
//
//void UGSBHubDetailView::HandleInventoryContextMenuCreated(UGSBContextMenu* ContextMenuWidget)
//{
//	if (FGSBContextMenuEntryOnClickedSignature* Signature = ContextMenuWidget->AddContextMenuEntry(TEXT("옮기기")))
//	{
//		Signature->AddDynamic(this, &UGSBHubDetailView::HandleContextMenuEntry_MoveItemToHubStorage);
//	}
//}
//
//void UGSBHubDetailView::HandleContextMenuEntry_MoveItemToHubStorage(UGSBContextMenuEntry* MenuEntry)
//{
//	//UGSBWindowSubsystem* WindowManager = GetGameInstance()->GetSubsystem<UGSBWindowSubsystem>();
//	//check(WindowManager);
//	//if (UGSBNumberInputDialogBody* DialogBody = WindowManager->OpenNumberInputDialog(MenuEntry->GetTargetObject(), 0))
//	//{
//	//	if (UGSBConfirmationDialog* Dialog = DialogBody->GetParentDialog<UGSBConfirmationDialog>())
//	//	{
//	//		Dialog->OnOKButtonClicked.AddDynamic(this, &UGSBHubDetailView::HandleOnDialogOKButtonClicked_MoveItemToHubStorage);
//	//	}
//	//}
//
//	//MenuEntry->GetParentContextMenu()->CloseContextMenu();
//}
//
//void UGSBHubDetailView::HandleOnDialogOKButtonClicked_MoveItemToHubStorage(UGSBConfirmationDialog* ConfirmationDialog)
//{
//	if (UGSBNumberInputDialogBody* DialogBody = ConfirmationDialog->GetDialogBody<UGSBNumberInputDialogBody>())
//	{
//		if (UGSBItemSlotWidget* ItemSlotWidget = Cast<UGSBItemSlotWidget>(DialogBody->GetTargetObject()))
//		{
//			FItemStack ItemStack;
//			ItemStack.ItemData = ItemSlotWidget->GetItemData();
//			ItemStack.Stack = DialogBody->GetNumber();
//			Inventory->MoveItemToStorage(HubStorage, ItemStack);
//		}
//	}
//}
