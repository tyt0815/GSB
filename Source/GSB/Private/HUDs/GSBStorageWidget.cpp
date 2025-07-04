// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBStorageWidget.h"
#include "HUDs/GSBItemListWidget.h"
#include "HUDs/GSBConfirmationDialog.h"
#include "Components/ItemStorageComponent.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "SubSystems/GSBWindowSubsystem.h"
#include "DebugHeader.h"

void UGSBStorageWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetIsFocusable(true);

	OnStorageContextWidgetCreated.AddDynamic(this, &UGSBStorageWidget::DefaultHandleOnStorageContextWidgetCreated);
}

void UGSBStorageWidget::UpdateWidget()
{
	if (IsValid(LinkedItemStorageComponent))
	{
		ItemList->ClearItemList();
		for (const FItemStack& ItemStack : LinkedItemStorageComponent->GetStoredItems())
		{
			if (FOnItemSlotClickedDelegates* OnClickedDelegates = ItemList->AddItemSlot(ItemStack))
			{
				OnClickedDelegates->OnItemSlotLeftClicked.AddDynamic(this, &UGSBStorageWidget::HandleItemSlotLeftClicked);
				OnClickedDelegates->OnItemSlotRightClicked.AddDynamic(this, &UGSBStorageWidget::ShowItemSlotContextMenu);
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

void UGSBStorageWidget::DefaultHandleOnStorageContextWidgetCreated(UGSBContextMenuWidget* ContextMenuWidget)
{
	AddContextMenuEntry_DeleteItem(ContextMenuWidget);
}

void UGSBStorageWidget::AddContextMenuEntry_DeleteItem(UGSBContextMenuWidget* ContextMenuWidget)
{
	if (IsValid(ContextMenuWidget))
	{
		if (FGSBContextMenuEntryOnClickedSignature* Signature = ContextMenuWidget->AddContextMenuEntry(TEXT("삭제")))
		{
			Signature->AddDynamic(this, &UGSBStorageWidget::DeleteItem);
		}
	}
}

void UGSBStorageWidget::HandleItemSlotLeftClicked(UGSBItemSlotWidget* ItemSlotWidget)
{
	OnItemSlotLeftClicked.Broadcast(ItemSlotWidget);
}

void UGSBStorageWidget::ShowItemSlotContextMenu(UGSBItemSlotWidget* ItemSlotWidget)
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (ContextMenuWidgetClass)
	{
		if (UGSBContextMenuWidget* ContextMenuWidget = CreateWidget<UGSBContextMenuWidget>(PlayerController, ContextMenuWidgetClass))
		{
			ContextMenuWidget->ClearContextMenuEntries();
			ContextMenuWidget->AddToViewport();

			float X;
			float Y;
			if (PlayerController->GetMousePosition(X, Y))
			{
				ContextMenuWidget->SetPositionInViewport(FVector2D(X, Y));
			}
			ContextMenuWidget->SetFocus();

			OnStorageContextWidgetCreated.Broadcast(ContextMenuWidget);

			ContextMenuWidget->SetContextTarget(ItemSlotWidget);
		}
	}
	else
	{
		TRACE_SCREEN_LOG(TEXT("ContextMenuWidgetClass가 nullptr 입니다."));
	}
}

void UGSBStorageWidget::DeleteItem(UWidget* ContextTarget, UGSBContextMenuEntry* MenuEntry)
{
	UGSBWindowSubsystem* WindowManager = GetGameInstance()->GetSubsystem<UGSBWindowSubsystem>();
	if (UGSBConfirmationDialog* Dialog = WindowManager->OpenConfirmationDialog())
	{
		
	}
	//if (UGSBItemSlotWidget* ItemSlotWidget = Cast<UGSBItemSlotWidget>(ContextTarget))
	//{
	//	FItemStack ItemStack = {};
	//	ItemStack.ItemData = ItemSlotWidget->GetItemData();
	//	ItemStack.Stack = 1;
	//	UnStoreItemStack(ItemStack);
	//}
}
