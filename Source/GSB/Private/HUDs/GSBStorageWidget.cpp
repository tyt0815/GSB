// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBStorageWidget.h"
#include "HUDs/GSBItemListWidget.h"
#include "HUDs/GSBConfirmationDialog.h"
#include "HUDs/GSBNumberInputDialogBody.h"
#include "HUDs/GSBContextMenuEntry.h"
#include "Components/ItemStorageComponent.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "SubSystems/GSBWindowSubsystem.h"
#include "DebugHeader.h"

void UGSBStorageWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetIsFocusable(true);

	OnStorageContextWidgetCreated.AddDynamic(this, &UGSBStorageWidget::AddDefaultContextMenuEntry);
}

void UGSBStorageWidget::UpdateWidget()
{
	ItemList->OnAttachedToStorageWidget(this);
	if (IsValid(LinkedItemStorageComponent))
	{
		ItemList->ClearItemList();
		for (const FItemStack& ItemStack : LinkedItemStorageComponent->GetStoredItems())
		{
			if (FOnItemSlotClickedDelegates* OnClickedDelegates = ItemList->AddItemSlot(ItemStack))
			{
				OnClickedDelegates->OnItemSlotLeftClicked.AddDynamic(this, &UGSBStorageWidget::HandleItemSlotLeftClicked);
				OnClickedDelegates->OnItemSlotRightClicked.AddDynamic(this, &UGSBStorageWidget::OpenItemSlotContextMenu);
			}
		}
	}
}

bool UGSBStorageWidget::TryLinkStorageComponent(UItemStorageComponent* StorageComponent)
{
	if (IsValid(StorageComponent))
	{
		return StorageComponent->TryLinkStorageWidget(this);
	}
	return false;
}

void UGSBStorageWidget::OnLinkedStorageComponent(UItemStorageComponent* StorageComponent)
{
	LinkedItemStorageComponent = StorageComponent;
	UpdateWidget();
}

void UGSBStorageWidget::SetTitle(const FText& NewTitle)
{
	Title->SetText(NewTitle);
}

FItemStack UGSBStorageWidget::GetItemStack(UItemDataAsset* ItemData) const
{
	if (IsValid(LinkedItemStorageComponent))
	{
		return LinkedItemStorageComponent->GetItemStack(ItemData);
	}
	return FItemStack();
}

int32 UGSBStorageWidget::StoreItemStack(const FItemStack& ItemStack)
{
	return LinkedItemStorageComponent->StoreItem(ItemStack);
}

int32 UGSBStorageWidget::UnStoreItemStack(const FItemStack& ItemStack)
{
	return LinkedItemStorageComponent->UnstoreItem(ItemStack);
}

int32 UGSBStorageWidget::MoveItemToStorage(UGSBStorageWidget* To, const FItemStack& ItemStack)
{
	if (IsValid(To) && IsValid(LinkedItemStorageComponent))
	{
		return LinkedItemStorageComponent->MoveItemTo(To->LinkedItemStorageComponent, ItemStack);
	}

	return 0;
}

int32 UGSBStorageWidget::MoveAllItemToStorage(UGSBStorageWidget* To, UItemDataAsset* ItemData)
{
	if (IsValid(To) && IsValid(LinkedItemStorageComponent))
	{
		return LinkedItemStorageComponent->MoveAllItemTo(To->LinkedItemStorageComponent, ItemData);
	}

	return 0;
}

void UGSBStorageWidget::AddDefaultContextMenuEntry(UGSBContextMenu* ContextMenuWidget)
{
	if (FGSBContextMenuEntryOnClickedSignature* Signature = ContextMenuWidget->AddContextMenuEntry(TEXT("삭제")))
	{
		Signature->AddDynamic(this, &UGSBStorageWidget::HandleContextMenuEntry_DeleteItem);
	}
}

void UGSBStorageWidget::HandleItemSlotLeftClicked(UGSBItemSlotWidget* ItemSlotWidget)
{
	OnItemSlotLeftClicked.Broadcast(ItemSlotWidget);
}

void UGSBStorageWidget::HandleOnConfirmDeleteButtonClicked(UGSBConfirmationDialog* ConfirmationDialog)
{
	if (UGSBNumberInputDialogBody* DialogBody = ConfirmationDialog->GetDialogBody<UGSBNumberInputDialogBody>())
	{
		if (UGSBItemSlotWidget* ItemSlotWidget = Cast<UGSBItemSlotWidget>(DialogBody->GetTargetObject()))
		{
			FItemStack ItemStack;
			ItemStack.ItemData = ItemSlotWidget->GetItemData();
			ItemStack.Stack = DialogBody->GetNumber();
			UnStoreItemStack(ItemStack);
		}
	}
}

void UGSBStorageWidget::OpenItemSlotContextMenu(UGSBItemSlotWidget* ItemSlotWidget)
{
	//UGSBWindowSubsystem* WindowManager = GetGameInstance()->GetSubsystem<UGSBWindowSubsystem>();
	//if (UGSBContextMenu* ContextMenuWidget = WindowManager->OpenContextMenu(ItemSlotWidget))
	//{
	//	OnStorageContextWidgetCreated.Broadcast(ContextMenuWidget);
	//}
}

void UGSBStorageWidget::HandleContextMenuEntry_DeleteItem(UGSBContextMenuEntry* MenuEntry)
{
	//UGSBWindowSubsystem* WindowManager = GetGameInstance()->GetSubsystem<UGSBWindowSubsystem>();
	//check(WindowManager);
	//UGSBItemSlotWidget* ItemSlot = Cast<UGSBItemSlotWidget>(MenuEntry->GetTargetObject());
	//check(ItemSlot);
	//FItemStack ItemStack = LinkedItemStorageComponent->GetItemStack(ItemSlot->GetItemData());
	//if (UGSBNumberInputDialogBody* DialogBody = WindowManager->OpenNumberInputDialog(ItemSlot, ItemStack.Stack))
	//{
	//	if (UGSBConfirmationDialog* Dialog = DialogBody->GetParentDialog<UGSBConfirmationDialog>())
	//	{
	//		Dialog->OnOKButtonClicked.AddDynamic(this, &UGSBStorageWidget::HandleOnConfirmDeleteButtonClicked);
	//	}
	//}
	//	
	//MenuEntry->GetParentContextMenu()->CloseContextMenu();
}
