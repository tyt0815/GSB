// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBInventoryWidget.h"
#include "HUDs/GSBContextMenuEntry.h"
#include "HUDs/GSBConfirmationDialog.h"
#include "HUDs/GSBNumberInputDialogBody.h"
#include "Components/InventoryComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "SubSystems/GSBWindowSubsystem.h"
#include "Items/DroppedItem.h"
#include "GSBGameInstance.h"
#include "DebugHeader.h"

void UGSBInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	OnStorageContextWidgetCreated.AddDynamic(this, &UGSBInventoryWidget::AddContextMenuEntry_DropItem);
}

void UGSBInventoryWidget::UpdateWidget()
{
	Super::UpdateWidget();

	UpdateWeightUI();
}

void UGSBInventoryWidget::UpdateWeightUI()
{
	if (UInventoryComponent* InventoryComponent = Cast<UInventoryComponent>(LinkedItemStorageComponent))
	{
		int32 MaxWeight = InventoryComponent->GetMaxWeight();
		int32 CurrentWeight = InventoryComponent->GetCurrentWeight();

		WeightLimitText->SetText(FText::FromString(FString::Printf(TEXT("(%d/%d)"), MaxWeight, CurrentWeight)));
		WeightProgressBar->SetPercent(static_cast<float>(CurrentWeight) / MaxWeight);
	}
}

void UGSBInventoryWidget::AddContextMenuEntry_DropItem(UGSBContextMenu* ContextMenuWidget)
{
	//if (FGSBContextMenuEntryOnClickedSignature* Signature = ContextMenuWidget->AddContextMenuEntry(TEXT("버리기")))
	//{
	//	Signature->AddDynamic(this, &UGSBInventoryWidget::HandleContextMenuEntry_DropItem);
	//}
}

void UGSBInventoryWidget::HandleContextMenuEntry_DropItem(UGSBContextMenuEntry* MenuEntry)
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
	//		Dialog->OnOKButtonClicked.AddDynamic(this, &UGSBInventoryWidget::HandleDialogOKButton_DropItem);
	//	}
	//}

	//MenuEntry->GetParentContextMenu()->CloseContextMenu();
}

void UGSBInventoryWidget::HandleDialogOKButton_DropItem(UGSBConfirmationDialog* ConfirmationDialog)
{
	if (UGSBNumberInputDialogBody* DialogBody = ConfirmationDialog->GetDialogBody<UGSBNumberInputDialogBody>())
	{
		if (UGSBItemSlotWidget* ItemSlotWidget = Cast<UGSBItemSlotWidget>(DialogBody->GetTargetObject()))
		{
			if (UGSBGameInstance* GameInst = GetGameInstance<UGSBGameInstance>())
			{
				FItemStack ItemStack;
				ItemStack.ItemData = ItemSlotWidget->GetItemData();
				ItemStack.Stack = DialogBody->GetNumber();
				ItemStack.Stack = UnStoreItemStack(ItemStack);
				if (UClass* DroppedItemClass = GameInst->GetActorClass(TEXT("DroppedItem")))
				{
					if (ADroppedItem* DroppedItem = GetWorld()->SpawnActor<ADroppedItem>(DroppedItemClass))
					{
						DroppedItem->SetActorLocation(GetOwningPlayerPawn()->GetActorLocation());
					}
				}
				else
				{
					TRACE_SCREEN_LOG(TEXT("DroppedItemClass가 nullptr입니다."));
				}
			}
		}
	}
}