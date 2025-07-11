// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBWindowBody.h"
#include "GSBHubDetailView.generated.h"

class UGSBStorageWidget;
class UGSBInventoryWidget;
class UGSBItemSlotWidget;
class UGSBContextMenu;
class UGSBContextMenuEntry;
class UGSBConfirmationDialog;
class UItemStorageComponent;
class UInventoryComponent;
class UItemDataAsset;

UCLASS()
class GSB_API UGSBHubDetailView : public UGSBWindowBody
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
public:
	void SetHubStorageAndInventory(UItemStorageComponent* HubStorageComponent, UItemStorageComponent* InventoryComponent);

private:
	UPROPERTY(meta = (BindWidget))
	UGSBStorageWidget* HubStorage;

	UPROPERTY(meta = (BindWidget))
	UGSBInventoryWidget* Inventory;

	UFUNCTION()
	void MoveItemFromHubStorageToInventory(UGSBItemSlotWidget* ItemSlotWidget);

	UFUNCTION()
	void MoveItemFromInventoryToHubStorage(UGSBItemSlotWidget* ItemSlotWidget);

	UFUNCTION()
	void HandleHubStorageContextMenuCreated(UGSBContextMenu* ContextMenuWidget);

	UFUNCTION()
	void HandleContextMenuEntry_MoveItemToInventory(UGSBContextMenuEntry* MenuEntry);

	UFUNCTION()
	void HandleOnDialogOKButtonClicked_MoveItemToInventory(UGSBConfirmationDialog* ConfirmationDialog);

	UFUNCTION()
	void HandleInventoryContextMenuCreated(UGSBContextMenu* ContextMenuWidget);

	UFUNCTION()
	void HandleContextMenuEntry_MoveItemToHubStorage(UGSBContextMenuEntry* MenuEntry);

	UFUNCTION()
	void HandleOnDialogOKButtonClicked_MoveItemToHubStorage(UGSBConfirmationDialog* ConfirmationDialog);
};
